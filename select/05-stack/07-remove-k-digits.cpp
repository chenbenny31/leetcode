// LC 402 Remove K Digits (Medium). Pattern: monotonic increasing stack, greedy.
// num is a non-negative integer string; remove k digits to get the smallest number
// (no leading zeros, "0" if nothing is left).

#include <iostream>
#include <string>

using std::cout;
using std::string;

// A bigger digit before a smaller one costs the most, so pop it while k lasts.
// T: O(n), S: O(n).
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

// Follow-up: largest number instead -> a smaller digit before a bigger one costs the
// most, so pop while top < digit. T: O(n), S: O(n).
string largestAfterRemoving(const string& num, int k) {
    // kept digits, non-increasing
    string stk;
    for (char c : num) {
        while (k > 0 && !stk.empty() && stk.back() < c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    // still non-increasing: the smallest digits sit at the end
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

void testLargest(const string& label, const string& num, int k, const string& want) {
    cout << label << (largestAfterRemoving(num, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("drop-peaks", "1532", 2, "12");
    test("leading-zeros", "3045", 1, "45");
    test("increasing", "1234", 2, "12");
    test("remove-all", "52", 2, "0");
    testLargest("largest-drop-dips", "1532", 2, "53");
    testLargest("largest-decreasing", "4321", 2, "43");
    testLargest("largest-leading-zeros", "0091", 2, "91");
    testLargest("largest-all-zero", "000", 1, "0");
}

// follow-up: remove duplicate letters (LC 316) -> same stack + last index of each char +
//   inStack; pop the top only if it appears again later
