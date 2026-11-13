// 11 · Top K Frequent Elements — LC 347, 692
// Mother: select/10-heap/02-top-k-frequent.cpp
// (1) LC 347: k most frequent values of nums, any order
// (2) stream of words: add(word); topK(k) = most frequent first, ties alphabetical
//
// clarify:
// k vs num of distinct values -> k <= distinct in (1); (2) returns fewer
// ties at the k-th place -> (1) any; (2) alphabetical
// output order -> (1) any; (2) most frequent first
// negative values -> ok, hash map

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

// (1) top k frequent (LC 347)
// solution 1: min-heap of size k; works for streams / small k
// core idea: cnt, keep the k best (cnt, value) in a min-heap: push, pop when size > k
// T: O(n log k), S: O(n)
vector<int> topKFrequent(const vector<int>& nums, int k) {
    // freq[value] = occurrences in nums
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    // heap = (cnt, value) of the k most frequent so far, least frequent on top
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

// solution 2: bucket sort; use it when O(n) is asked
// core idea: cnt <= n -> put each value in the bucket of its cnt, read from n down
// T: O(n), S: O(n)
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    int n = nums.size();
    // buckets[cnt] = values occurring exactly cnt times
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

// (2) top k in a stream of words
// core idea: set ranked by (-cnt, word) -> begin() = most frequent, ties alphabetical
// erase the old key before changing a cnt
// T: O(log W) add, O(k) topK, S: O(W) — W = distinct words
class TopKStream {
    // counts[word] = occurrences so far
    unordered_map<string, int> counts;
    // ranked = (-cnt, word) per seen word; begin() = most frequent, then alphabetical
    set<pair<int, string>> ranked;

public:
    void add(const string& word) {
        int& count = counts[word];
        // drop the stale key before the cnt changes
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

// sorted copy, to compare in any order
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

// follow-up:
// top k words, ties by word (LC 692) -> sort (word, cnt) by cnt desc, word asc
// bottom-k in the stream -> same set ranked by (cnt, word)
// top-k IPs, log too big for memory -> hash-partition into files, cnt each, one k-heap
// k largest files -> no counting, same size-k min-heap on (size, path)
// most talkative / active users -> sum each line's word cnt per user, then the heap
// O(n) average w/o buckets -> quickselect (nth_element) on (cnt, value)
// next word predictor -> 47
