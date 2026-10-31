// ByteDance (FastPrep #53, full-time phone): given a positive integer n and a set of
// distinct decimal digits, build the largest non-negative integer strictly smaller than
// n using only those digits (each digit may be reused). A multi-digit number cannot
// start with 0, but the one-digit number 0 is fine. Return -1 if no such number exists.
// 1 <= n <= 1e9, 1 <= digits.length <= 10.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::to_string;
using std::stoi;

// A same-length answer always beats a shorter one, so try that first: copy n's digits
// from the left while they are allowed, and at each position see whether a smaller
// allowed digit fits there. The further right that position is, the larger the result;
// after it, every digit is the largest allowed one. If no position works, use one digit
// fewer, all of them the largest digit. T: O(L * 10) for L digits of n, S: O(L).
int largestNumberBelow(int n, const vector<int>& digits) {
    string s = to_string(n);
    int len = s.size();
    // allowed[d] = digit d may be used
    bool allowed[10] = {};
    int maxDigit = 0;
    for (int d : digits) {
        allowed[d] = true;
        maxDigit = max(maxDigit, d);
    }

    // bestPos = latest position where a smaller allowed digit (bestDigit) can go
    int bestPos = -1;
    int bestDigit = -1;
    for (int i = 0; i < len; i++) {
        int limit = s[i] - '0';
        for (int d = limit - 1; d >= 0; d--) {
            bool leadingZero = i == 0 && d == 0 && len > 1;
            if (allowed[d] && !leadingZero) {
                bestPos = i;
                bestDigit = d;
                break;
            }
        }
        // the prefix can only continue while n's own digit is allowed
        if (!allowed[limit]) {
            break;
        }
    }

    if (bestPos != -1) {
        string result = s.substr(0, bestPos);
        result += char('0' + bestDigit);
        result += string(len - bestPos - 1, char('0' + maxDigit));
        return stoi(result);
    }

    // one digit fewer; if 0 is the only digit, nothing longer than "0" can be built
    if (len == 1) {
        return -1;
    }
    if (maxDigit == 0) {
        return 0;
    }
    return stoi(string(len - 1, char('0' + maxDigit)));
}

void test(const string& label, int n, const vector<int>& digits, int want) {
    cout << label << (largestNumberBelow(n, digits) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("backtrack", 5321, {1, 3, 5}, 5315);
    test("first-digit", 4000, {3, 9}, 3999);
    test("shorter", 1000, {1, 5}, 555);
    test("no-leading-zero", 30, {0, 4}, 4);
    test("zero-only", 25, {0}, 0);
    test("none", 1, {1, 5}, -1);
}
