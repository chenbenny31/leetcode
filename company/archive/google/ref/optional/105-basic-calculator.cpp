// 105 · Basic Calculator — LC 224
// Mother: select/05-stack/06-basic-calculator.cpp
// evaluate a valid expression of non-negative ints, + - ( ) and spaces; '-' may be unary
// e.g. "-(2+3)" = -5; no eval()
//
// clarify:
// operators -> only + and -, no * or /
// unary minus -> allowed
// input valid? -> yes
// int overflow -> assume result fits in int

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::pair;
using std::string;
using std::vector;

// core idea: only + and -, so keep running sum, num being read and the sign before it
// at '(' save (outer sum, sign before paren); at ')' fold the inner sum back in
// T: O(n), S: O(depth)
int calculate(const string& s) {
    int result = 0;
    int num = 0;
    int sign = 1;
    // saved[d] = (outer result, sign before '(') for each open paren
    vector<pair<int, int>> saved;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '+' || c == '-') {
            result += sign * num;
            num = 0;
            sign = c == '+' ? 1 : -1;
        } else if (c == '(') {
            saved.push_back({result, sign});
            result = 0;
            sign = 1;
        } else if (c == ')') {
            result += sign * num;
            num = 0;
            auto [outer, outerSign] = saved.back();
            saved.pop_back();
            // paren's value becomes one signed term of the outer sum
            result = outer + outerSign * result;
        }
    }
    return result + sign * num;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spaces", " 7 - 3 + 1 ", 5);
    test("minus-paren", "5-(2-3)", 6);
    test("nested", "2-(3-(4+1))", 4);
    test("unary-minus", "-(2+3)", -5);
}

// follow-up:
// add * and / (LC 772) -> recursion: run the LC 227 loop until ')' or the end
// '(' in LC 772 -> recurse; its value becomes the current num
