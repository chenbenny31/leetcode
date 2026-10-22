// LC 907 Sum of Subarray Minimums: return the sum of min(subarray) over all contiguous
// subarrays of arr, modulo 1e9 + 7.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// contribution counting: arr[mid] is the minimum of (mid - left) * (right - mid)
// subarrays, where left is the previous smaller and right the next smaller-or-equal
// (the asymmetry counts equal values once). Monotonic stack finds both in one pass.
// T: O(n), S: O(n).
int sumSubarrayMins(const vector<int>& arr) {
    const int MOD = 1000000007;
    int n = arr.size();
    vector<int> stk;   // indices with strictly increasing values
    long long sum = 0;
    for (int i = 0; i <= n; i++) {
        // i == n flushes the stack; values are positive so 0 is below all of them
        int cur = i < n ? arr[i] : 0;
        while (!stk.empty() && arr[stk.back()] >= cur) {
            int mid = stk.back();
            stk.pop_back();
            int left = stk.empty() ? -1 : stk.back();
            sum = (sum + 1LL * arr[mid] * (mid - left) * (i - mid)) % MOD;
        }
        stk.push_back(i);
    }
    return sum;
}

void test(const string& label, const vector<int>& arr, int want) {
    if (sumSubarrayMins(arr) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("single", {7}, 7);
    test("duplicates", {2, 2, 2}, 2 * 6);
    test("mixed", {3, 1, 2}, 3 + 1 + 2 + 1 + 1 + 1);
    test("modulo", vector<int>(30000, 30000), 449905500);
}
