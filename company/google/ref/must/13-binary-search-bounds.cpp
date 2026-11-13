// 13 · First and Last Position (Binary Search Bounds) — LC 34, 35, 69, 278
// Mother problem: select/04-binary-search/01-first-last-position.cpp
// Google reports: sqrt, prefix count ×2, prefix words ×2, point-in-range ×2 and interval
// membership (42), random pick with weight (15) ×3, max profit job scheduling (83)
//
// LC 34: nums sorted ascending; [first, last] index of target, [-1, -1] if absent, in
// O(log n).
// Clarify
// - duplicates → yes, a run is the point; empty → [-1, -1]; values span the int range
// Core idea
// - One template: lo = 0, hi = n, while lo < hi, mid = lo + (hi - lo) / 2; "none" = n.
// - first = lowerBound (first >= target), last = upperBound (first > target) - 1.
// - lower tests nums[mid] < target, upper nums[mid] <= target; check lo < n and
//   nums[lo] == target before using lo. lowerBound(target + 1) overflows at INT_MAX.
// - T O(log n), S O(1).
// Tests: run of duplicates; missing in a gap; empty; target INT_MAX at the end.
//
// Follow-ups
// - search insert (LC 35) → lowerBound; count of target → upperBound - lowerBound
// - integer sqrt (LC 69) → upperBound loop on m in [0, x + 1] with mid * mid <= x in
//   long long, return lo - 1
// - first bad version (LC 278) → same template on [1, n], hi = mid when isBad(mid)
// - any monotonic predicate → replace nums[mid] < target with !pred(mid)
// - closest value → lowerBound, then compare it with the one before (gaps in long long)
// - floor / ceiling → upperBound - 1 / lowerBound, -1 when out of range
// - count sorted words with a prefix → lowerBound(prefix + '{') - lowerBound(prefix)
// - target indices after sorting (LC 2089) → count < target and == target, no sort
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <climits>

using std::cout;
using std::string;
using std::vector;

// Lower-bound template: first index i with nums[i] >= target (n if none).
// The answer always lies in [lo, hi]; hi starts at n so "none" is a valid answer.
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

// Upper-bound template: first index i with nums[i] > target (n if none).
// Identical loop to lowerBound, only < becomes <=. T: O(log n), S: O(1).
int upperBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] <= target) {
            // mid and everything left of it are <= target, not the answer
            lo = mid + 1;
        } else {
            // mid is a candidate; the answer is mid or left of it
            hi = mid;
        }
    }
    return lo;
}

// first = lowerBound(target), last = upperBound(target) - 1. T: O(log n), S: O(1).
vector<int> searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int first = lowerBound(nums, target);
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
    test("missing-gap", {1, 3}, 2, {-1, -1});
    test("empty", {}, 1, {-1, -1});
    test("int-max-at-end", {1, INT_MAX, INT_MAX}, INT_MAX, {1, 2});
}
