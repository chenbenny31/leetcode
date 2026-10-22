// LC 402 Remove K Digits: remove k digits from the non-negative number string num so the
// result is the smallest possible. No leading zeros in the result ("0" if empty).
#include <iostream>
#include <string>
using std::cout;
using std::string;

// Monotonic increasing stack: a bigger digit just before a smaller one is dropped first.
// T: O(n), S: O(n).
string removeKdigits(const string& num, int k) {
    string stk;   // kept digits, non-decreasing from bottom to top
    for (char c : num) {
        while (k > 0 && !stk.empty() && stk.back() > c) {
            stk.pop_back();
            k--;
        }
        stk.push_back(c);
    }
    // leftover removals come off the largest end of a non-decreasing string
    while (k > 0 && !stk.empty()) {
        stk.pop_back();
        k--;
    }
    int n = stk.size();
    int start = 0;
    while (start < n && stk[start] == '0') { start++; }
    if (start == n) { return "0"; }
    return stk.substr(start);
}

void test(const string& label, const string& num, int k, const string& want) {
    if (removeKdigits(num, k) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("drop-peaks", "3142", 2, "12");
    test("leading-zeros", "10200", 1, "200");
    test("remove-all", "12", 2, "0");
    test("increasing-trim-tail", "1234", 1, "123");
}
