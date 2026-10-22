// LC 410 Split Array Largest Sum: split nums (non-negative) into k non-empty contiguous
// subarrays so that the largest subarray sum is minimized. Return that sum.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Greedy: the fewest subarrays needed so that no subarray sum exceeds cap.
// T: O(n), S: O(1).
int piecesNeeded(const vector<int>& nums, int cap) {
    int pieces = 1;
    int sum = 0;   // sum of the current piece
    for (int x : nums) {
        if (sum + x > cap) {
            pieces++;
            sum = 0;
        }
        sum += x;
    }
    return pieces;
}

// Binary search on the answer in [max element, total]; feasible = pieces <= k, which is
// monotonic in cap. T: O(n * log(sum)), S: O(1).
int splitArray(const vector<int>& nums, int k) {
    int lo = 0;
    int hi = 0;
    for (int x : nums) {
        lo = max(lo, x);
        hi += x;
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (piecesNeeded(nums, mid) <= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    if (splitArray(nums, k) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("two-parts", {1, 2, 3, 4}, 2, 6);
    test("one-part", {1, 2, 3, 4}, 1, 10);
    test("each-alone", {1, 2, 3, 4}, 4, 4);
    test("single", {5}, 1, 5);
}
