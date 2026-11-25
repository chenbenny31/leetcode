// k <= distinct
// tie break: any or alpha
// order: any or most freq first
// neg vals -> need hash-map

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

// min-heap of size-k, drop min on top if hit cap, keep k max
// works for stream/small-k, T: O(nlogk), S: O(n) for the map, O(k) heap
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freq; // freq[val] = cnt
    for (int num : nums) { freq[num]++; }
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> heap; // (cnt, val) top by min cnt
    for (const auto& [val, cnt] : freq) {
        heap.push({cnt, val});
        if ((int)heap.size() > k) {
            heap.pop();
        }
    }

    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// bucket sort, put value of cnt into bucket[cnt], read from n downwards
// T: O(n), S: O(n)
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    int n = nums.size();

    vector<vector<int>> buckets(n + 1); // buckets[cnt] = [vals,]
    for (const auto& [val, cnt] : freq) {
        buckets[cnt].push_back(val);
    }

    vector<int> result;
    for (int cnt = n; cnt >= 1; cnt--) {
        for (int val : buckets[cnt]) {
            result.push_back(val);
            if ((int)result.size() == k) { return result; }
        }
    }
    return result;
}

// set ranked by (-cnt, word), begin() = most-freq, ties on alph
// T: O(logW) add, O(k) topk, S: O(W), W=uniq-words
class TopKStream {
private:
    unordered_map<string, int> freq; // freq[word] = cnt
    set<pair<int, string>> rank; // (-cnt, word), begin by most freq

public:
    void add(const string& word) { // T: O(logn)
        int& cnt = freq[word];
        if (cnt > 0) { rank.erase({-cnt, word}); }
        cnt++;
        rank.insert({-cnt, word});
    }

    vector<string> topK(int k) {
        vector<string> result;
        auto it = rank.begin();
        while (it != rank.end() && (int)result.size() < k) {
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
