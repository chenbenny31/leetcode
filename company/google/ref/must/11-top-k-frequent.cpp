// 11 · Top K Frequent Elements — LC 347, 692
// Mother problem: select/10-heap/02-top-k-frequent.cpp
// Google reports: top-k IPs ×3, LC 347 ×4, largest files ×2, talkative users ×2, stream,
// first / last k words ×2, next word predictor (47) ×3, most active users ×2
//
// LC 347: the k most frequent values of nums, any order; the answer is unique.
// Clarify
// - n up to 1e5, 1 <= k <= distinct values; negatives fine (only hash keys)
// - output order → any here; most frequent first for LC 692 and the stream
// Core idea
// - Count in a hash map, keep the k best (count, value) in a min-heap: push, then pop
//   when size > k (popping first drops a kept value).
// - Buckets indexed by count (n + 1 slots, a count can be n) give O(n).
// - T O(n log k) heap / O(n) buckets, S O(n).
// Tests: two clear winners; k == distinct values; negatives; stream tie / overtake.
//
// Follow-ups
// - top k words, ties by word (LC 692) → sort (word, count) by count desc, word asc
// - streaming top-k → `TopKStream` below
// - bottom-k in the stream → the same set ranked by (count, word)
// - top-k IPs in a log too big for memory → hash-partition IPs into files, count one
//   file at a time, merge into one size-k heap (an IP lives in exactly one file)
// - k largest files → no counting, the same size-k min-heap on (size, path)
// - most talkative users → add each line's word count per user, then the same heap
// - O(n) average without buckets → nth_element on (count, value) with greater<>
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <set>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::priority_queue;
using std::set;
using std::greater;
using std::sort;

// Count, then keep the k most frequent (count, value) in a min-heap.
// T: O(n log k), S: O(n).
vector<int> topKFrequent(const vector<int>& nums, int k) {
    // freq[value] = occurrences in nums
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    // (count, value) of the k most frequent so far, least frequent on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (const auto& [val, count] : freq) {
        heap.push({count, val});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// Bucket sort: a count is at most n, so scan buckets from high to low.
// T: O(n), S: O(n).
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    // freq[value] = occurrences in nums
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    int n = nums.size();
    // buckets[count] = values occurring exactly count times
    vector<vector<int>> buckets(n + 1);
    for (const auto& [val, count] : freq) { buckets[count].push_back(val); }
    vector<int> result;
    for (int count = n; count >= 1; count--) {
        for (int val : buckets[count]) {
            result.push_back(val);
            if ((int)result.size() == k) { return result; }
        }
    }
    return result;
}

// Streaming top-k words, ties alphabetical: a set ranked by (-count, word), so begin()
// is the most frequent. T: O(log W) per add, O(k) per topK; S: O(W), W = distinct words.
class TopKStream {
    // counts[word] = occurrences so far
    unordered_map<string, int> counts;
    // (-count, word) for each seen word; begin() = most frequent, then alphabetical
    set<pair<int, string>> ranked;

public:
    void add(const string& word) {
        int& count = counts[word];
        // drop the stale key before the count changes
        if (count > 0) { ranked.erase({-count, word}); }
        count++;
        ranked.insert({-count, word});
    }

    vector<string> topK(int k) {
        vector<string> result;
        auto it = ranked.begin();
        while (it != ranked.end() && (int)result.size() < k) {
            result.push_back(it->second);
            it++;
        }
        return result;
    }
};

// canonical form for comparing: any order is accepted
vector<int> sorted(vector<int> v) {
    sort(v.begin(), v.end());
    return v;
}

void test(const string& label, const vector<int>& nums, int k, const vector<int>& want) {
    bool ok = sorted(topKFrequent(nums, k)) == sorted(want) &&
              sorted(topKFrequentBucket(nums, k)) == sorted(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testStream(const string& label, const vector<string>& words, int k,
                const vector<string>& want) {
    TopKStream stream;
    for (const string& word : words) { stream.add(word); }
    cout << label << (stream.topK(k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("top-two", {4, 4, 4, 6, 6, 9}, 2, {4, 6});
    test("all", {3, 8}, 2, {3, 8});
    test("negative", {-2, -2, 5}, 1, {-2});
    testStream("stream-tie-by-name", {"pear", "fig", "pear", "apple", "fig"}, 2,
               {"fig", "pear"});
    testStream("stream-overtake", {"cat", "dog", "dog"}, 1, {"dog"});
    testStream("stream-k-above-distinct", {"bee", "bee", "ant"}, 3, {"bee", "ant"});
}
