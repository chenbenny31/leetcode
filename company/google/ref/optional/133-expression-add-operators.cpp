// 133 · Expression Add Operators — LC 282
// digit string num and a target; insert binary +, -, * between digits (multi-digit ok)
// return every expression that evaluates to target, any order
//
// clarify:
// unary minus -> no
// leading zeros -> "05" invalid, "0" fine
// operand size -> long long

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::to_string;
using std::sort;

// core idea: dfs over operand splits w/ value (result so far) and last (last signed term)
// * undoes last: value - last + last * cur; '-' stores last = -cur
// T: O(n * 4^n), S: O(n) — 4 choices per gap (none, +, -, *), n per string built
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

// follow-up:
// + and * w/ parentheses (Google variant) -> interval DP: reachable values per range
