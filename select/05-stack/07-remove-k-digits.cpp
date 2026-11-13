// LC 402 Remove K Digits (Medium) — monotonic increasing stack, greedy
// remove k digits from num string to get the smallest number; no leading zeros
//
// clarify:
// k range -> k <= len of num
// leading zeros -> none in num; strip them from the result
// nothing left (k = len) -> "0"
// num size -> too long for an integer, stay on the string

#include <iostream>
#include <string>

using std::cout;
using std::string;

// core idea: a bigger digit before a smaller one costs most -> pop it while k lasts
// removals still left come off the end
// T: O(n), S: O(n)
string removeKdigits(const string& num, int k) {
    // stk = kept digits, non-decreasing
    string stk;
    for (char c : num) {
        while (k > 0 && !stk.empty() && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    // still non-decreasing: largest digits sit at the end
    while (k > 0 && !stk.empty()) {
        stk.pop_back();
        k--;
    }
    int start = 0;
    while (start < (int)stk.size() && stk[start] == '0') { start++; }
    string result = stk.substr(start);
    return result.empty() ? "0" : result;
}

void test(const string& label, const string& num, int k, const string& want) {
    cout << label << (removeKdigits(num, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("drop-peaks", "1532", 2, "12");
    test("leading-zeros", "3045", 1, "45");
    test("increasing", "1234", 2, "12");
    test("remove-all", "52", 2, "0");
    return 0;
}

// follow-up:
// largest number instead -> mirror: pop while top < digit, then cut leftover k at the end
// remove dup letters, smallest result (LC 316) -> same stack + last idx + inStack
// smallest subsequence of len (LC 1673) -> same stack w/ k = n - len removals
// largest number from two arrays (LC 321) -> per split: best i + len - i digits, merge
