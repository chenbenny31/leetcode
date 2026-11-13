// LC 282 Expression Add Operators: digit string num and a target. Insert binary +, -, *
// between digits (operands may be multi-digit) and return every expression that
// evaluates to target, in any order.
//
// Clarify:
// - no unary minus; no leading zeros ("05" invalid, "0" fine)
// - operands up to 10 digits -> long long for operands and values
// Idea: dfs(pos, expr, value, last): value = result so far, last = the last signed term,
//   so * can undo it. For each end, operand cur = num[pos..end]:
//   '+' -> (value + cur, cur); '-' -> (value - cur, -cur);
//   '*' -> (value - last + last * cur, last * cur). At pos == 0 start with cur alone.
// Pitfalls:
// - stop extending the operand once it starts with '0' and has more than one digit
// - '-' stores last = -cur so a later * multiplies the signed term
// Tests to write: + and * both hit; leading zero excluded; all three ops on "00";
//   no answer on a long string.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::to_string;
using std::sort;

// out collects every expression equal to target
void dfs(const string& num, long long target, int pos, const string& expr,
         long long value, long long last, vector<string>& out) {
    if (pos == (int)num.size()) {
        if (value == target) { out.push_back(expr); }
        return;
    }
    long long cur = 0;
    for (int end = pos; end < (int)num.size(); end++) {
        // no leading zeros: "0" is the only operand starting with '0'
        if (end > pos && num[pos] == '0') { break; }
        cur = cur * 10 + (num[end] - '0');
        string operand = to_string(cur);
        if (pos == 0) {
            dfs(num, target, end + 1, operand, cur, cur, out);
            continue;
        }
        dfs(num, target, end + 1, expr + "+" + operand, value + cur, cur, out);
        dfs(num, target, end + 1, expr + "-" + operand, value - cur, -cur, out);
        long long product = last * cur;
        dfs(num, target, end + 1, expr + "*" + operand, value - last + product, product,
            out);
    }
}

// try every operand split and operator. T: O(n * 4^n), S: O(n) recursion.
vector<string> addOperators(const string& num, int target) {
    vector<string> out;
    dfs(num, target, 0, "", 0, 0, out);
    return out;
}

void test(const string& label, const string& num, int target, vector<string> want) {
    vector<string> got = addOperators(num, target);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plus-times", "123", 6, {"1+2+3", "1*2*3"});
    test("leading-zero", "105", 5, {"1*0+5", "10-5"});
    test("zeros", "00", 0, {"0+0", "0-0", "0*0"});
    test("none", "3456237490", 9191, {});
}

// follow-up: + and * with parentheses (Google variant) -> interval DP: the set of
//   reachable values for each digit range, combine left / right sets by + and *
