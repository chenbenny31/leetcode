// LC 224 Basic Calculator (Hard) — running sum + stack of (outer sum, sign) per '('
// evaluate valid expression of non-negative ints, + - ( ) and spaces; no eval()
//
// clarify:
// input valid? -> yes
// unary minus -> possible, e.g. "-(2+3)"; reads as 0 - x
// multi-digit nums -> yes
// overflow -> assume result fits in int

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::pair;
using std::string;
using std::vector;

// core idea: only + and -, so keep a running sum
// at '(' save outer sum + sign before the paren; at ')' fold inner sum back as one term
// T: O(n), S: O(n)
int calculate(const string& s) {
    // result = sum of finished terms at current depth
    int result = 0;
    int num = 0;
    // sign = sign before num
    int sign = 1;
    // saved = one (outer result, sign before '(') per open paren
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
            // the paren's value becomes one signed term of the outer sum
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
    return 0;
}

// follow-up:
// + - * / w/o parens (LC 227) -> 02-basic-calculator-ii: keep the last term apart
// + - * / w/ parens (LC 772) -> recursion: calc(s, pos) runs LC 227 loop until ')' or end
// variables w/ given values -> look the name up in a map where a number is read
