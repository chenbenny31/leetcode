// LC 347: return the k most frequent elements of nums, in any order.
// The answer is unique.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::sort;
using std::priority_queue;
using std::greater;

// straightforward: count, sort (count, value) pairs by count descending, take k.
// T: O(n log n), S: O(n).
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;   // freq[value] = occurrences in nums
    for (int x : nums) { freq[x]++; }
    vector<pair<int, int>> byCount;   // (count, value), one per distinct value
    for (const auto& [val, cnt] : freq) { byCount.push_back({cnt, val}); }
    // reverse iterators: ascending order read backwards = largest count first
    sort(byCount.rbegin(), byCount.rend());
    vector<int> result;
    for (int i = 0; i < k; i++) { result.push_back(byCount[i].second); }
    return result;
}

// improved (k << n, or a stream): min-heap of the k best (count, value) seen so far;
// a new pair evicts the least frequent kept one. T: O(n log k), S: O(n).
vector<int> topKFrequentHeap(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;   // freq[value] = occurrences in nums
    for (int x : nums) { freq[x]++; }
    // min-heap of (count, value): top = least frequent of the kept k
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (const auto& [val, cnt] : freq) {
        heap.push({cnt, val});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// improved: a count is at most n, so bucket values by count and scan buckets from
// high to low; no sort. T: O(n), S: O(n).
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;   // freq[value] = occurrences in nums
    for (int x : nums) { freq[x]++; }
    int n = nums.size();
    vector<vector<int>> buckets(n + 1);   // buckets[c] = values occurring exactly c times
    for (const auto& [val, cnt] : freq) { buckets[cnt].push_back(val); }
    vector<int> result;
    for (int c = n; c >= 1; c--) {
        for (int val : buckets[c]) {
            result.push_back(val);
            // stop at exactly k even if a tie splits this bucket; exits both loops
            if ((int)result.size() == k) { return result; }
        }
    }
    return result;
}

// canonical form for comparing: any order is accepted
vector<int> sorted(vector<int> v) {
    sort(v.begin(), v.end());
    return v;
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    bool ok = sorted(topKFrequent(nums, k)) == sorted(want) &&
              sorted(topKFrequentHeap(nums, k)) == sorted(want) &&
              sorted(topKFrequentBucket(nums, k)) == sorted(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 1, 2, 2, 2, 3}, 2, {1, 2});
    test("all", {1, 2}, 2, {1, 2});
    test("neg", {-1, -1, 2}, 1, {-1});
}
