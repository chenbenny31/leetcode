// LC 402 Remove K Digits (Medium) — monotonic increasing stack, greedy
// num is a non-negative integer string; remove k digits to get the smallest number
// (no leading zeros, "0" if nothing is left).
// Clarify: k <= length of num; num has no leading zeros; k == length gives "0"; num too
//   long to convert to an integer.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Core idea: a bigger digit before a smaller one costs the most, so pop it from the stack
// while k lasts; removals still left come off the end. T: O(n), S: O(n).
string removeKdigits(const string& num, int k) {
    // kept digits, non-decreasing
    string stk;
    for (char c : num) {
        while (k > 0 && !stk.empty() && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    // still non-decreasing: the largest digits sit at the end
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

// follow-up (ideas only):
// - largest number instead -> mirror: pop while top < digit (non-increasing stack), then
//   cut what is left of k from the end
// - remove duplicate letters, smallest result (LC 316) -> same stack + last index of each
//   char + inStack; pop the top only if it appears again later
// - smallest subsequence of length len (LC 1673) -> same stack with k = n - len removals
// - largest number from two arrays keeping order (LC 321) -> for each split, take the
//   best i digits of one and len - i of the other with this stack, then merge greedily
