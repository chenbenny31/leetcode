// LC 224 Basic Calculator: evaluate a valid expression of non-negative integers, '+',
// '-', '(', ')' and spaces. '-' may be unary, e.g. "-(2+3)". No eval().
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

// One pass with a running sum and a sign; at '(' stash the outer state on a stack and
// start fresh, at ')' fold the paren's value back as one signed term. T: O(n), S: O(n).
int calculate(const string& s) {
    int result = 0;   // sum of finished terms at the current depth
    int num = 0;      // number being read
    int sign = 1;     // sign in front of num
    // one entry per open paren: (result so far outside it, sign in front of it)
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
            result = outer + outerSign * result;
        }
    }
    return result + sign * num;
}

void test(const string& label, const string& s, int want) {
    cout << label << (calculate(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spaces", " 10 - 4 + 3 ", 9);
    test("nested", "10-(2+(3-4))", 9);
    test("unary-minus", "-(2+3)", -5);
    test("double-negate", "2-(5-6)", 3);
}
