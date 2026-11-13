// 13 · First and Last Position (Binary Search Bounds) — LC 34, 35, 69, 278
// Mother: select/04-binary-search/01-first-last-position.cpp
// LC 34: nums sorted ascending; [first, last] idx of target in O(log n), else [-1, -1]
//
// clarify:
// dups -> yes, that is the point
// empty array -> [-1, -1]
// value range -> target + 1 can overflow, so use upperBound(target) instead

#include <iostream>
#include <vector>
#include <string>
#include <climits>

using std::cout;
using std::string;
using std::vector;

// core idea: first idx i w/ nums[i] >= target (n if none); answer always in [lo, hi]
// hi starts at n -> "none" is a valid answer
// T: O(log n), S: O(1)
int lowerBound(const vector<int>& nums, int target) {
    int lo = 0;
    int hi = nums.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] < target) {
            lo = mid + 1;
        } else {
            // mid is a candidate -> answer is mid or left of it
            hi = mid;
        }
    }
    return lo;
}

// core idea: first idx i w/ nums[i] > target (n if none); lowerBound w/ < replaced by <=
// T: O(log n), S: O(1)
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

// core idea: first = lowerBound(target), last = upperBound(target) - 1
// T: O(log n), S: O(1)
vector<int> searchRange(const vector<int>& nums, int target) {
    int n = nums.size();
    int first = lowerBound(nums, target);
    if (first == n || nums[first] != target) {
        return {-1, -1};
    }
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

// follow-up:
// search insert (LC 35) -> lowerBound; cnt of target -> upperBound - lowerBound
// integer sqrt (LC 69) -> upper-bound loop on [0, x + 1] w/ mid * mid <= x in long long
// first bad version (LC 278) -> same template on [1, n], hi = mid when isBad(mid)
// any monotonic predicate -> replace nums[mid] < target w/ !pred(mid)
// closest value -> lowerBound, compare w/ the one before (gaps in long long)
// floor / ceiling -> upperBound - 1 / lowerBound, -1 when out of range
// cnt sorted words w/ a prefix -> lowerBound(prefix + '{') - lowerBound(prefix)
// interval membership -> 42; random pick w/ weight -> 15; max profit job scheduling -> 83
