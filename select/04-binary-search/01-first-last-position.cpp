// LC 34 Find First and Last Position of Element in Sorted Array (Medium). Pattern:
// lower-bound template.
// nums sorted ascending; return [first, last] index of target, or [-1, -1], in O(log n).

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

// Rule: the loop only returns a position. Before using it, check the index is in
// range (lowerBound: lo < n; upperBound: lo > 0, then read lo - 1) and the value
// there equals target.

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

// Count of target = length of [lowerBound, upperBound); 0 if absent.
// T: O(log n), S: O(1).
int countOf(const vector<int>& nums, int target) {
    return upperBound(nums, target) - lowerBound(nums, target);
}

// LC 35: index of target, or where it would be inserted = lowerBound.
// T: O(log n), S: O(1).
int searchInsert(const vector<int>& nums, int target) {
    return lowerBound(nums, target);
}

// LC 69: last m with m * m <= x = (first m with m * m > x) - 1, the upperBound loop on
// m in [0, x + 1]; long long so mid * mid never overflows. T: O(log x), S: O(1).
int mySqrt(int x) {
    // the first m with m * m > x lies in [lo, hi]; hi = x + 1 always qualifies
    long long lo = 0;
    long long hi = (long long)x + 1;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (mid * mid <= x) {
            // mid and everything left of it are <= x, not the answer
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    // lo >= 1 since 0 * 0 <= x, so lo - 1 is in range
    return (int)(lo - 1);
}

void test(const string& label, const vector<int>& nums, int target,
          const vector<int>& want) {
    cout << label << (searchRange(nums, target) == want ? " PASS\n" : " FAIL\n");
}

void testBounds(const string& label, const vector<int>& nums, int target,
                int wantUpper, int wantCount, int wantInsert) {
    bool ok = upperBound(nums, target) == wantUpper
              && countOf(nums, target) == wantCount
              && searchInsert(nums, target) == wantInsert;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSqrt(const string& label, int x, int want) {
    cout << label << (mySqrt(x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups", {1, 2, 2, 2, 3}, 2, {1, 3});
    test("all-same", {2, 2}, 2, {0, 1});
    test("missing-gap", {1, 3}, 2, {-1, -1});
    test("empty", {}, 1, {-1, -1});
    test("dups-at-end", {1, 2, 2, 2}, 2, {1, 3});
    test("int-max", {1, INT_MAX}, INT_MAX, {1, 1});
    testBounds("bounds-above-all", {1, 3, 3}, 9, 3, 0, 3);
    testBounds("bounds-below-all", {1, 3, 3}, 0, 0, 0, 0);
    testBounds("bounds-gap", {1, 3, 3}, 2, 1, 0, 1);
    testBounds("bounds-dups-at-end", {1, 2, 2, 2}, 2, 4, 3, 1);
    testSqrt("sqrt-zero", 0, 0);
    testSqrt("sqrt-perfect", 16, 4);
    testSqrt("sqrt-between", 15, 3);
    testSqrt("sqrt-int-max", INT_MAX, 46340);
}

// follow-up: last = lowerBound(target + 1) - 1 also works, but only for integers, and
//   target + 1 overflows at INT_MAX; upperBound(target) - 1 has neither problem
// follow-up: "first index where pred is true" for any monotonic pred -> same template,
//   replace nums[mid] < target with !pred(mid)
// follow-up: first bad version (LC 278) -> lower bound on the predicate over [1, n]:
//   isBadVersion(mid) ? hi = mid : lo = mid + 1
// follow-up: target indices after sorting (LC 2089) -> no sort: smaller = count of
//   values < target, equal = count == target; answer smaller .. smaller + equal - 1
