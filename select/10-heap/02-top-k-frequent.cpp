// LC 347 Top K Frequent Elements (Medium) — count map + heap / buckets / quickselect
// return the k most frequent values of nums, any order
//
// clarify:
// k range -> k <= num of distinct values
// ties at k-th place -> assume answer unique
// output order -> any
// negative values -> ok, map keys

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

// solution 1: count + size-k min-heap; default, good when k << n
// core idea: keep k best (cnt, value) in a min-heap; top = weakest kept, leaves at k + 1
// T: O(n log k), S: O(n)
vector<int> topKFrequent(const vector<int>& nums, int k) {
    // value -> cnt
    unordered_map<int, int> count;
    for (int num : nums) { count[num]++; }
    // (cnt, value), smallest cnt on top, at most k pairs
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

// solution 2: bucket by cnt; O(n) worst case
// core idea: cnt <= n -> buckets[cnt] holds values; read from n down until k taken
// T: O(n), S: O(n)
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

// solution 3: quickselect on (cnt, value); avg O(n), no buckets
// core idea: partition around random pivot, larger cnt first; go to side w/ idx k - 1
// T: O(n), S: O(n) — avg, worst O(n^2)
vector<int> topKFrequentQuickselect(const vector<int>& nums, int k) {
    unordered_map<int, int> count;
    for (int num : nums) { count[num]++; }
    // distinct values as (cnt, value)
    vector<pair<int, int>> items;
    for (const auto& [value, freq] : count) { items.push_back({freq, value}); }
    int lo = 0;
    int hi = (int)items.size() - 1;
    while (lo < hi) {
        swap(items[lo + rand() % (hi - lo + 1)], items[hi]);
        int pivot = items[hi].first;
        // items[lo..store) have cnt > pivot's
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

// follow-up:
// top-k words, ties by smaller word (LC 692) -> heap of (cnt, word), weakest on top
// stream of adds, query top k any time -> set of (-cnt, word); erase old key first
// top-k IPs in a file too big for memory -> hash-partition into shards, top k each, merge
// k-th largest value itself (LC 215) -> 01-kth-largest (size-k heap or quickselect)
