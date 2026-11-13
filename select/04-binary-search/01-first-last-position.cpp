// LC 34 Find First and Last Position of Element in Sorted Array (Medium) — two bounds
// nums is sorted ascending; return [first, last] index of target, or [-1, -1] if it is
// absent, in O(log n).
// Clarify: duplicates (a run of equal values); empty array; 0-based indices; values
//   over the whole int range.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: first index with nums[i] >= target (n if none). Search the half-open range
// [lo, hi): everything before lo is < target, everything from hi on is >= target.
// T: O(log n), S: O(1).
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            // mid and everything left of it are too small
            lo = mid + 1;
        } else {
            // mid is a candidate; the answer is mid or left of it
            hi = mid;
        }
    }
    return lo;
}

// Core idea: first index with nums[i] > target (n if none); the same loop as lowerBound
// with < replaced by <=. T: O(log n), S: O(1).
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

// Core idea: first = lowerBound(target); if that index is in range and holds target,
// last = upperBound(target) - 1. T: O(log n), S: O(1).
vector<int> searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int first = lowerBound(nums, target);
    // the loop only returns a position: check the range, then the value
    if (first == n || nums[first] != target) {
        return {-1, -1};
    }
    // the last target sits just before the first value > target
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

// follow-up (ideas only):
// - count of target / insert position (LC 35) -> upperBound - lowerBound / lowerBound
// - floor, ceiling, closest value -> floor = upperBound - 1 (none if it is 0), ceiling =
//   lowerBound (none if it is n); closest = the nearer of the two, gaps in long long
// - sqrt(x) (LC 69) -> upperBound loop on m in [0, x + 1) with test m * m <= x in long
//   long; answer lo - 1
// - first bad version (LC 278), any monotonic predicate -> same loop with !pred(mid) as
//   the first test; mid = lo + (hi - lo) / 2 because n can be INT_MAX
// - last = lowerBound(target + 1) - 1 -> works for ints only and target + 1 overflows at
//   INT_MAX; upperBound(target) - 1 has neither problem
// - count sorted words with a prefix -> two lower bounds on strings, [prefix,
//   prefix + '{'); see 15-trie/01-implement-trie
// - target indices after sorting (LC 2089) -> no sort: count the values smaller than and
//   equal to target; the answer is smaller .. smaller + equal - 1
// - plain binary search (LC 704) -> lowerBound, then check i < n && nums[i] == target
// - search a 2D matrix (LC 74) -> treat it as one sorted array of m * n values; index
//   mid is cell (mid / n, mid % n)
// - matrix sorted by rows and by columns (LC 240) -> staircase walk from the top-right:
//   too big moves left, too small moves down, O(m + n)
