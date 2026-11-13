// 100 · Basic Calculator — LC 224
// Mother problem: select/05-stack/06-basic-calculator.cpp
//
// Evaluate a valid expression of non-negative ints, + - ( ) and spaces; '-' may be unary,
// e.g. "-(2+3)". No eval().
//
// Clarify
// - operators? → + − ( ) and spaces, non-negative ints; no * or /
// - unary minus? → yes, e.g. "-(2+3)"
// - valid input? → assume always valid
// - overflow? → result fits in int
//
// Core idea
// - Only + and −, so keep a running `result`, the current `num` and the `sign` before
//   it.
// - At '(' push (result, sign) and reset; at ')' fold: result = outer + outerSign ·
//   inner.
// - A leading '-' works because result starts at 0.
// - T O(n), S O(depth).
//
// Tests
// - spaces " 7 - 3 + 1 " → 5
// - minus before paren "5-(2-3)" → 6
// - nested "2-(3-(4+1))" → 4
// - unary minus "-(2+3)" → −5
//
// Follow-ups
// - add * and / (LC 772) → recursion: calc(s, i) runs the LC 227 loop until ')' or the
//   end; '(' recurses and its value becomes num (see `calculateWithParens` in
//   `select/05-stack/02-basic-calculator-ii.cpp`)

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::pair;
using std::string;
using std::vector;

// Only + and -, so keep a running sum; at '(' save the outer sum and the sign before the
// paren, at ')' fold the inner sum back in. T: O(n), S: O(n).
int calculate(const string& s) {
    // sum of finished terms at the current depth
    int result = 0;
    // number being read
    int num = 0;
    // sign before num
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
}
