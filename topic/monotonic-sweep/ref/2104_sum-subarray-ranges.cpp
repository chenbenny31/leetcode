// LC 2104 Sum of Subarray Ranges: the range of a subarray is max - min. Return the sum
// of the ranges of all contiguous subarrays of nums.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// contribution counting with a monotonic stack: arr[mid] is the minimum of
// (mid - left) * (right - mid) subarrays (left = previous smaller, right = next
// smaller-or-equal so ties count once). T: O(n), S: O(n).
long long sumOfMins(const vector<int>& arr) {
    int n = arr.size();
    vector<int> stk;   // indices with strictly increasing values
    long long sum = 0;
    for (int i = 0; i <= n; i++) {
        // i == n flushes the stack
        while (!stk.empty() && (i == n || arr[stk.back()] >= arr[i])) {
            int mid = stk.back();
            stk.pop_back();
            int left = stk.empty() ? -1 : stk.back();
            sum += 1LL * arr[mid] * (mid - left) * (i - mid);
        }
        stk.push_back(i);
    }
    return sum;
}

// sum of ranges = sum of maxes - sum of mins; the maxes of nums are the mins of -nums.
// T: O(n), S: O(n).
long long subArrayRanges(const vector<int>& nums) {
    vector<int> neg;
    for (int x : nums) { neg.push_back(-x); }
    return -sumOfMins(neg) - sumOfMins(nums);
}

void test(const string& label, const vector<int>& nums, long long want) {
    if (subArrayRanges(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("single", {5}, 0);
    test("equal", {2, 2, 2}, 0);
    test("negatives", {-1, 3, -2}, 14);
    test("increasing", {1, 2, 3}, 1 + 1 + 2);
}
