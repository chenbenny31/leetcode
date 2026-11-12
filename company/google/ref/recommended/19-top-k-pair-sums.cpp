// Top K Pair Sums (Google, interview report): given nums (n up to 1e5) and k, return the
// k largest values of nums[i] + nums[j] over index pairs i < j, in descending order.
// Equal values at different indices are different pairs. Assume 1 <= k <= n(n-1)/2.

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

void test(const string& label, const vector<int>& nums, int k,
          const vector<long long>& want) {
    bool ok = topKPairSumsAll(nums, k) == want && topKPairSums(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {5, 1, 3}, 2, {8, 6});
    test("all-pairs", {1, 2, 3}, 3, {5, 4, 3});
    test("duplicates", {2, 2, 2}, 3, {4, 4, 4});
    test("negatives", {-1, 4, -3, 2}, 4, {6, 3, 1, 1});
}

// follow-up: drop the visited set -> seed (i, i + 1) for every i and only ever move
//   right; each pair then has one parent. T: O(n log n + k log n), S: O(n).
// follow-up: only the k-th largest -> binary search on the sum value; count pairs with
//   sum >= mid by two pointers on the sorted array. T: O(n log n + n log range).
// follow-up: K largest subset sums (LC 2386) -> same expand-from-the-best heap: start at
//   the sum of positives, sort absolute values ascending as a; heap of (sum, idx) from
//   (best - a[0], 0); pop, then push (sum - a[idx + 1], idx + 1) and
//   (sum + a[idx] - a[idx + 1], idx + 1). The first answer is best itself.
// follow-up: pairs across two sorted arrays (LC 373) -> same heap, neighbors (i + 1, j)
//   and (i, j + 1)
