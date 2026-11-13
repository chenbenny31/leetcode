// Top K Pair Sums (Google, interview report)
// return the k largest nums[i] + nums[j] over idx pairs i < j, descending; n up to 1e5
//
// clarify:
// how large is k -> 1 <= k <= n(n-1)/2; the n^2 / 2 pairs cannot all be built
// i < j only -> yes, no i == j
// equal values at different idx -> different pairs
// value range, negatives -> any int; sums in long long
// sums or idx pairs -> sums; idx pairs = follow-up

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <utility>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::set;
using std::tuple;
using std::pair;
using std::sort;
using std::greater;

// solution 1: brute force; small n only, use as a checker
// core idea: every pair's sum, sort descending, keep k
// T: O(n^2 log n), S: O(n^2)
vector<long long> topKPairSumsAll(const vector<int>& nums, int k) {
    int n = nums.size();
    vector<long long> sums;
    for (int left = 0; left < n; left++) {
        for (int right = left + 1; right < n; right++) {
            sums.push_back(1LL * nums[left] + nums[right]);
        }
    }
    sort(sums.begin(), sums.end(), greater<long long>());
    sums.resize(k);
    return sums;
}

// solution 2: sort + max-heap of pairs; the answer for large n
// core idea: sort descending -> (0, 1) is best, moving either idx right never raises sum
// pop the best pair, push its 2 neighbors (left, right + 1) and (left + 1, right)
// T: O(n log n + k log k), S: O(n + k)
vector<long long> topKPairSums(vector<int> nums, int k) {
    int n = nums.size();
    sort(nums.begin(), nums.end(), greater<int>());
    // (sum, left, right), largest sum on top
    priority_queue<tuple<long long, int, int>> heap;
    // idx pairs already pushed -> each pair enters the heap once
    set<pair<int, int>> visited;
    heap.push({1LL * nums[0] + nums[1], 0, 1});
    visited.insert({0, 1});
    vector<long long> result;
    while ((int)result.size() < k && !heap.empty()) {
        auto [sum, left, right] = heap.top();
        heap.pop();
        result.push_back(sum);
        if (right + 1 < n && visited.insert({left, right + 1}).second) {
            heap.push({1LL * nums[left] + nums[right + 1], left, right + 1});
        }
        if (left + 1 < right && visited.insert({left + 1, right}).second) {
            heap.push({1LL * nums[left + 1] + nums[right], left + 1, right});
        }
    }
    return result;
}

// cnt pairs i < j of ascending nums w/ nums[i] + nums[j] >= target (two pointers)
long long countPairsAtLeast(const vector<int>& nums, long long target) {
    int left = 0;
    int right = nums.size() - 1;
    long long count = 0;
    while (left < right) {
        if (1LL * nums[left] + nums[right] >= target) {
            count += right - left;
            right--;
        } else {
            left++;
        }
    }
    return count;
}

// follow-up: only the k-th largest sum, k up to n(n-1)/2
// core idea: binary search on the value; answer = largest s w/ >= k pairs of sum >= s
// T: O(n log n + n log R), S: O(1) — R = max sum - min sum; S beyond the sorted copy
long long kthLargestPairSum(vector<int> nums, long long k) {
    sort(nums.begin(), nums.end());
    int n = nums.size();
    // invariant: at least k pairs have sum >= low; answer in [low, high]
    long long low = 1LL * nums[0] + nums[1];
    long long high = 1LL * nums[n - 2] + nums[n - 1];
    while (low < high) {
        // round up, or low = mid never makes progress
        long long mid = low + (high - low + 1) / 2;
        if (countPairsAtLeast(nums, mid) >= k) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

// follow-up: LC 2386, k largest subset sums, empty subset included
// core idea: best = sum of positives; any sum = best - subset sum of |values| a (sorted)
// state (sum, idx): a[idx] = largest taken off; children add a[idx + 1] or swap to it
// each subset has one parent -> no visited set
// T: O(n log n + k log k), S: O(n + k)
vector<long long> topKSubsetSums(const vector<int>& nums, int k) {
    long long best = 0;
    // absolute values, ascending after the sort
    vector<long long> a;
    for (int x : nums) {
        if (x > 0) { best += x; }
        a.push_back(x > 0 ? x : -1LL * x);
    }
    sort(a.begin(), a.end());
    int n = a.size();
    // (subset sum, idx of largest absolute value taken off), largest sum on top
    priority_queue<pair<long long, int>> heap;
    if (n > 0) { heap.push({best - a[0], 0}); }
    vector<long long> result = {best};
    while ((int)result.size() < k && !heap.empty()) {
        auto [sum, idx] = heap.top();
        heap.pop();
        result.push_back(sum);
        if (idx + 1 < n) {
            heap.push({sum - a[idx + 1], idx + 1});
            heap.push({sum + a[idx] - a[idx + 1], idx + 1});
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k,
          const vector<long long>& want) {
    bool ok = topKPairSumsAll(nums, k) == want && topKPairSums(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testKth(const string& label, const vector<int>& nums, long long k, long long want) {
    cout << label << (kthLargestPairSum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

void testSubsets(const string& label, const vector<int>& nums, int k,
                 const vector<long long>& want) {
    cout << label << (topKSubsetSums(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {5, 1, 3}, 2, {8, 6});
    test("all-pairs", {1, 2, 3}, 3, {5, 4, 3});
    test("duplicates", {2, 2, 2}, 3, {4, 4, 4});
    test("negatives", {-1, 4, -3, 2}, 4, {6, 3, 1, 1});

    testKth("kth-last-pair", {5, 1, 3}, 3, 4);
    testKth("kth-tied", {-1, 4, -3, 2}, 4, 1);
    testKth("kth-equal", {2, 2, 2}, 2, 4);
    testKth("kth-negatives", {-1, 4, -3, 2}, 5, -1);

    testSubsets("subset-mixed", {3, -1, 2}, 5, {5, 4, 3, 2, 2});
    testSubsets("subset-all-negative", {-2, -5}, 4, {0, -2, -5, -7});
    testSubsets("subset-zeros", {0, 0}, 4, {0, 0, 0, 0});
}

// follow-up:
// drop the visited set -> seed (i, i + 1) for every i, only move right; one parent each
// cost w/o visited set -> T: O(n log n + k log n), S: O(n)
// only the k-th largest sum (k up to n^2 / 2) -> kthLargestPairSum above
// k largest subset sums (LC 2386) -> topKSubsetSums above
// k smallest pairs across two ascending arrays (LC 373) -> min-heap from (0, 0)
// LC 373 step -> pop (i, j), push (i + 1, j) and (i, j + 1), each once (visited set)
// return original idx pairs -> sort an idx array by value descending, map back on output
