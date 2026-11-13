// LC 34 Find First and Last Position of Element in Sorted Array (Medium) — two bounds
// nums sorted ascending; return [first, last] idx of target, [-1, -1] if absent; O(log n)
//
// clarify:
// dups -> yes, a run of equal values
// empty array -> [-1, -1]
// idx -> 0-based
// value range -> whole int, so no target + 1 tricks

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// first idx w/ nums[i] >= target, n if none; half-open [lo, hi)
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            // mid and everything left of it are too small
            lo = mid + 1;
        } else {
            // mid is a candidate; answer is mid or left of it
            hi = mid;
        }
    }
    return lo;
}

// first idx w/ nums[i] > target, n if none; lowerBound w/ < replaced by <=
int upperBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] <= target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

// core idea: first = lowerBound(target); if it holds target, last = upperBound - 1
// T: O(log n), S: O(1)
vector<int> searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int first = lowerBound(nums, target);
    // lowerBound only returns a position: check range, then value
    if (first == n || nums[first] != target) {
        return {-1, -1};
    }
    // last target sits just before the first value > target
    int last = upperBound(nums, target) - 1;
    return {first, last};
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (searchRange(nums, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups", {1, 2, 2, 2, 3}, 2, {1, 3});
    test("dups-at-end", {1, 2, 2}, 2, {1, 2});
    test("missing-gap", {1, 3}, 2, {-1, -1});
    test("empty", {}, 1, {-1, -1});
    return 0;
}

// follow-up:
// cnt of target / insert position (LC 35) -> upperBound - lowerBound / lowerBound
// floor / ceiling -> floor = upperBound - 1 (none if 0), ceiling = lowerBound (none if n)
// sqrt(x) (LC 69) -> upperBound loop on m in [0, x + 1) w/ m * m <= x (long long); lo - 1
// first bad version (LC 278), any monotonic pred -> same loop w/ !pred(mid) as first test
// last = lowerBound(target + 1) - 1 -> ints only, overflows at INT_MAX
// cnt sorted words w/ a prefix -> two lower bounds: [prefix, prefix + '{')
// search a 2D matrix (LC 74) -> one sorted array of m * n; mid = cell (mid / n, mid % n)
// rows and cols sorted (LC 240) -> staircase from top-right: big -> left, small -> down
