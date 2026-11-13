// LC 347 Top K Frequent Elements (Medium) — count map + heap / buckets / quickselect
// Return the k most frequent values of nums, in any order.
// Clarify: k <= number of distinct values; ties at the k-th place; output order;
//   negative values.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::priority_queue;
using std::greater;
using std::sort;
using std::swap;
using std::rand;

// Core idea: count, then keep the k best (count, value) pairs in a min-heap; the top is
// the weakest kept, so it leaves when a new pair makes the size k + 1.
// T: O(n log k), S: O(n).
vector<int> topKFrequent(const vector<int>& nums, int k) {
    // value -> occurrences
    unordered_map<int, int> count;
    for (int num : nums) { count[num]++; }
    // (count, value), smallest count on top, at most k pairs
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (const auto& [value, freq] : count) {
        heap.push({freq, value});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// Core idea: a count is at most n, so put each value in the bucket of its count and read
// the buckets from n down until k values are taken. T: O(n), S: O(n).
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> count;
    for (int num : nums) { count[num]++; }
    // buckets[c] = values that occur exactly c times
    vector<vector<int>> buckets(nums.size() + 1);
    for (const auto& [value, freq] : count) { buckets[freq].push_back(value); }
    vector<int> result;
    for (int freq = nums.size(); freq >= 1 && (int)result.size() < k; freq--) {
        for (int value : buckets[freq]) {
            if ((int)result.size() < k) { result.push_back(value); }
        }
    }
    return result;
}

// Core idea: quickselect on the (count, value) pairs: partition around a random pivot
// with larger counts first, and recurse only into the side that holds position k - 1.
// T: O(n) average, O(n^2) worst, S: O(n).
vector<int> topKFrequentQuickselect(const vector<int>& nums, int k) {
    unordered_map<int, int> count;
    for (int num : nums) { count[num]++; }
    // distinct values as (count, value)
    vector<pair<int, int>> items;
    for (const auto& [value, freq] : count) { items.push_back({freq, value}); }
    int lo = 0;
    int hi = (int)items.size() - 1;
    while (lo < hi) {
        swap(items[lo + rand() % (hi - lo + 1)], items[hi]);
        int pivot = items[hi].first;
        // items[lo..store) have a count above the pivot's
        int store = lo;
        for (int i = lo; i < hi; i++) {
            if (items[i].first > pivot) {
                swap(items[i], items[store]);
                store++;
            }
        }
        swap(items[store], items[hi]);
        if (store == k - 1) { break; }
        if (store < k - 1) {
            lo = store + 1;
        } else {
            hi = store - 1;
        }
    }
    vector<int> result;
    for (int i = 0; i < k; i++) { result.push_back(items[i].second); }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, vector<int> want) {
    vector<int> byHeap = topKFrequent(nums, k);
    vector<int> byBucket = topKFrequentBucket(nums, k);
    vector<int> bySelect = topKFrequentQuickselect(nums, k);
    sort(byHeap.begin(), byHeap.end());
    sort(byBucket.begin(), byBucket.end());
    sort(bySelect.begin(), bySelect.end());
    sort(want.begin(), want.end());
    bool ok = byHeap == want && byBucket == want && bySelect == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-winners", {5, 5, 5, 7, 7, 9}, 2, {5, 7});
    test("k-equals-distinct", {4, 6, 4}, 2, {4, 6});
    test("negative-values", {-3, -3, 8}, 1, {-3});
    test("single", {2}, 1, {2});
    return 0;
}

// follow-up (ideas only):
// - top-k words, ties by smaller word (LC 692) -> heap of (count, word) whose comparator
//   keeps the weakest (lower count, then larger word) on top; reverse at the end
// - stream of additions, query top k any time -> set of (-count, word); erase the old key
//   before changing a count
// - top-k IPs in a file too big for memory -> hash-partition lines into shards, count and
//   take the top k per shard, merge the shard winners
// - k-th largest value itself (LC 215) -> 01-kth-largest (size-k heap or quickselect)
