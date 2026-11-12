// LC 347 Top K Frequent Elements (Medium). Pattern: count + size-k min-heap / buckets.
// Return the k most frequent values of nums, in any order. The answer is unique.
//
// Clarify:
// - n up to 1e5, 1 <= k <= distinct values -> O(n log k) heap or O(n) buckets; sorting
//   the distinct values, O(n log n), is also accepted
// - ties at the k-th place -> LC 347 promises none; LC 692 breaks them by smaller word
// - output order -> any for LC 347; most frequent first for LC 692 and the stream
// - negatives -> fine, values are only hash keys
// - k above the distinct count -> return all of them (the stream does; ask otherwise)
// Idea: count with a hash map, then keep the k best (count, value) pairs in a min-heap:
//   the top is the weakest kept, so each new pair goes in and the weakest comes out.
//   A count is at most n, so buckets indexed by count give O(n).
// Pitfalls:
// - min-heap (greater) capped at k, not a max-heap of every value (O(n log n))
// - push (count, value), not (value, count): pairs compare the first field first
// - push first, then pop when size > k; popping first drops a kept value even when the
//   new one is weaker
// - buckets need n + 1 slots: a count can be n
// - LC 692 order: count desc, then word asc; a size-k heap must keep the reverse on top
// - TopKStream: erase the old (-count, word) key before changing the count
// Tests to write: two clear winners; k == distinct values; negative values; word ties by
//   name; a stream where one word overtakes another.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <set>
#include <sstream>
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
using std::istringstream;
using std::hash;
using std::reverse;

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

// LC 692: count, then sort (word, count) by count desc, word asc and take the first k.
// T: O(n log n), S: O(n). A size-k heap with the reversed order gives O(n log k).
vector<string> topKFrequentWords(const vector<string>& words, int k) {
    // freq[word] = occurrences in words
    unordered_map<string, int> freq;
    for (const string& word : words) { freq[word]++; }
    // (word, count) entries, ranked by the sort below
    vector<pair<string, int>> entries(freq.begin(), freq.end());
    sort(entries.begin(), entries.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             if (a.second != b.second) { return a.second > b.second; }
             return a.first < b.first;
         });
    vector<string> result;
    for (int i = 0; i < k; i++) { result.push_back(entries[i].first); }
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

// follow-up (Google ×3: top-k IPs in a log too big for memory): each line starts with
// an IP. Hash-partition the IPs into buckets (files on disk in a real run), count one
// bucket at a time, and merge into one size-k heap: an IP lives in exactly one bucket, so
// its count there is final. Most hits first, ties by the smaller IP string.
// Pitfall: the heap keeps the weakest on top; with (-hits, ip) the default max-heap puts
// the fewest hits, then the largest ip, on top, so ties keep the smaller ip.
// T: O(L + U log k), S: O(L) here, O(U / partitions + k) in memory per bucket in a real
// run; L = log size, U = distinct IPs.
vector<string> topKIps(const vector<string>& logLines, int k, int partitions) {
    // buckets[b] = the IP of each line whose IP hashes to b
    vector<vector<string>> buckets(partitions);
    for (const string& line : logLines) {
        istringstream in(line);
        string ip;
        in >> ip;
        buckets[hash<string>{}(ip) % partitions].push_back(ip);
    }
    // (-hits, ip) of the best k so far; top = fewest hits, then the largest ip
    priority_queue<pair<int, string>> weakest;
    for (const vector<string>& bucket : buckets) {
        // hits[ip] = lines from ip; only this bucket's IPs are in memory
        unordered_map<string, int> hits;
        for (const string& ip : bucket) { hits[ip]++; }
        for (const auto& [ip, count] : hits) {
            weakest.push({-count, ip});
            if ((int)weakest.size() > k) { weakest.pop(); }
        }
    }
    vector<string> result;
    while (!weakest.empty()) {
        result.push_back(weakest.top().second);
        weakest.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}

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

void testWords(const string& label, const vector<string>& words, int k,
               const vector<string>& want) {
    bool ok = topKFrequentWords(words, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testStream(const string& label, const vector<string>& words, int k,
                const vector<string>& want) {
    TopKStream stream;
    for (const string& word : words) { stream.add(word); }
    cout << label << (stream.topK(k) == want ? " PASS\n" : " FAIL\n");
}

void testIps(const string& label, const vector<string>& logLines, int k, int partitions,
             const vector<string>& want) {
    cout << label << (topKIps(logLines, k, partitions) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("top-two", {4, 4, 4, 6, 6, 9}, 2, {4, 6});
    test("all", {3, 8}, 2, {3, 8});
    test("negative", {-2, -2, 5}, 1, {-2});
    testWords("words-tie-by-name", {"pear", "apple", "pear", "apple", "fig"}, 2,
              {"apple", "pear"});
    testWords("words-count-first", {"zoo", "zoo", "ant"}, 2, {"zoo", "ant"});
    testWords("words-cut-at-k", {"cat", "bee", "dog", "bee"}, 2, {"bee", "cat"});
    testStream("stream-tie-by-name", {"pear", "fig", "pear", "apple", "fig"}, 2,
               {"fig", "pear"});
    testStream("stream-overtake", {"cat", "dog", "dog"}, 1, {"dog"});
    testStream("stream-k-above-distinct", {"bee", "bee", "ant"}, 3, {"bee", "ant"});

    vector<string> log = {"10.0.0.1 GET /a", "10.0.0.2 GET /b", "10.0.0.1 POST /c",
                          "10.0.0.3 GET /a", "10.0.0.2 GET /a", "10.0.0.1 GET /d"};
    testIps("ips-top-two", log, 2, 3, {"10.0.0.1", "10.0.0.2"});
    testIps("ips-one-partition", log, 2, 1, {"10.0.0.1", "10.0.0.2"});
    testIps("ips-tie-by-ip", {"9.9.9.9 x", "1.1.1.1 y", "5.5.5.5 z"}, 2, 2,
            {"1.1.1.1", "5.5.5.5"});
    testIps("ips-k-above-distinct", {"2.2.2.2 a", "2.2.2.2 b", "3.3.3.3 c"}, 5, 4,
            {"2.2.2.2", "3.3.3.3"});
}

// follow-up: O(n) average without buckets -> quickselect on (count, value) pairs; after
//   it the first k entries are the answer:
//     vector<pair<int, int>> entries;   // (count, value), one per distinct value
//     for (const auto& [val, count] : freq) { entries.push_back({count, val}); }
//     nth_element(entries.begin(), entries.begin() + k - 1, entries.end(),
//                 greater<pair<int, int>>());
// follow-up: bottom-k in the stream -> rank by (count, word), begin() = least frequent:
//     if (count > 0) { ranked.erase({count, word}); }
//     count++;
//     ranked.insert({count, word});
// follow-up: top-k IPs in a GB-scale log file (×3) -> topKIps above
// follow-up: k largest files (×2) -> no counting; the same size-k min-heap on
//   (size, path):
//     heap.push({size, path});
//     if ((int)heap.size() > k) { heap.pop(); }
// follow-up: most talkative users (×2), lines "user word word ..." -> parse like topKIps
//   and add the line's word count instead of 1:
//     istringstream in(line);
//     in >> user;
//     while (in >> word) { wordsBy[user]++; }
