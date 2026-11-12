// Top K Pair Sums (Google, interview report): given nums (n up to 1e5) and k, return the
// k largest values of nums[i] + nums[j] over index pairs i < j, in descending order.
// Equal values at different indices are different pairs. Assume 1 <= k <= n(n-1)/2.
//
// Clarify:
// - n up to 1e5 -> n^2 / 2 = 5e9 pairs: never build them all; k up to ~1e5 -> heap
// - pairs i < j: each unordered pair once, never an index with itself
// - equal values at different indices -> separate pairs (stated), so equal sums repeat
// - values up to ~1e9, negatives allowed -> sums need long long
// - k larger than the number of pairs -> assumed not (else return every pair)
// - return the sums only, descending (index pairs: follow-up)
// Idea: sorted descending, the sum only drops when either index moves right, so (0, 1)
//   is the best pair and the next best is always a neighbor of a pair already popped.
//   Pop the best, push its two neighbors: the heap grows by at most one per pop, so k
//   pops cost O(k log k).
// Pitfalls:
// - (1, 3) is a neighbor of both (0, 3) and (1, 2): mark visited on push, or it enters
//   the heap twice and its sum is output twice
// - left + 1 < right keeps i < j; right + 1 < n is the other bound
// - 1LL * nums[a] + nums[b]: int + int overflows before it is widened
// - sort descending (greater<int>()); priority_queue on a tuple is a max-heap by sum
// Tests to write: a few distinct values; k = every pair; all values equal (repeated
//   sums); negatives with a tied sum.

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

// straightforward: every pair's sum, sort descending, keep k. T: O(n^2 log n), S: O(n^2).
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

// improved: sort descending, so (0, 1) is the best pair and moving either index right
// never increases the sum. Pop the best pair, push its two neighbors (left, right + 1)
// and (left + 1, right). T: O(n log n + k log k), S: O(n + k).
vector<long long> topKPairSums(vector<int> nums, int k) {
    int n = nums.size();
    sort(nums.begin(), nums.end(), greater<int>());
    // (sum, left, right), largest sum on top
    priority_queue<tuple<long long, int, int>> heap;
    // index pairs already pushed, so each pair enters the heap once
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

// number of pairs i < j in the ascending array with nums[i] + nums[j] >= target. Two
// pointers from the ends: if the outer pair reaches target, so does every left index
// up to right with this right. T: O(n), S: O(1).
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

// follow-up: only the k-th largest sum, k up to n(n-1)/2 (far too many pops). Binary
// search on the value: the answer is the largest s with at least k pairs of sum >= s.
// T: O(n log n + n log R), R = max sum - min sum; S: O(1) beyond the sorted copy.
// Pitfalls: low = mid needs mid rounded up, or the search never ends; k and the count
//   are long long (n^2 / 2 overflows int).
// Tests to write: the last pair; a tied sum; all values equal; negatives.
long long kthLargestPairSum(vector<int> nums, long long k) {
    sort(nums.begin(), nums.end());
    int n = nums.size();
    // invariant: at least k pairs have sum >= low; the answer is in [low, high]
    long long low = 1LL * nums[0] + nums[1];
    long long high = 1LL * nums[n - 2] + nums[n - 1];
    while (low < high) {
        long long mid = low + (high - low + 1) / 2;
        if (countPairsAtLeast(nums, mid) >= k) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

// follow-up (LC 2386): the k largest subset sums, descending; the empty subset counts
// (sum 0). best = the sum of the positives is the largest. Every subset sum is best
// minus a subset sum of the absolute values (drop a positive or add a negative), so
// expand from best as above: with a = absolute values ascending, state (sum, idx) has
// a[idx] as the largest value taken off; its children take off a[idx + 1] too, or take
// it off instead of a[idx]. Each subset of a has one parent, so no visited set.
// T: O(n log n + k log k), S: O(n + k). Assume 1 <= k <= 2^n.
// Pitfalls: best itself is the first answer (nothing taken off) and is not in the heap;
//   the absolute values must be sorted ascending; sums in long long.
// Tests to write: mixed signs with a repeated sum; all negative -> 0 first; zeros.
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
    // (subset sum, index of the largest absolute value taken off), largest sum on top
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

// follow-up: drop the visited set -> seed (i, i + 1) for every i and only ever move
//   right; each pair then has one parent. T: O(n log n + k log n), S: O(n).
//     for (int i = 0; i + 1 < n; i++) {
//         heap.push({1LL * nums[i] + nums[i + 1], i, i + 1});
//     }
//     after a pop: if (right + 1 < n) { push (left, right + 1) }
// follow-up: only the k-th largest (k up to n^2 / 2) -> kthLargestPairSum above
// follow-up: K largest subset sums (LC 2386) -> topKSubsetSums above
// follow-up: k smallest pairs across two ascending arrays (LC 373) -> min-heap from
//   (0, 0); after popping (i, j) push (i + 1, j) and (i, j + 1), each once:
//     if (i + 1 < m && visited.insert({i + 1, j}).second) { push (i + 1, j) }
//     if (j + 1 < n && visited.insert({i, j + 1}).second) { push (i, j + 1) }
// follow-up: return the original index pairs -> sort an index array order by value
//   descending instead of nums; heap entries are positions in order; output
//   {min(order[left], order[right]), max(order[left], order[right])}
