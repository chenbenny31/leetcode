// return the k most frequent elems of nums, in any order

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::sort;
using std::priority_queue;
using std::greater;

// minHeap with {cnt, val}, T: O(n log k), S: O(n) (freq map; heap is O(k))
vector<int> topKFrequent(const vector<int>& nums, int k) {
    unordered_map<int, int> freq; // freq[val] = cnt of val
    for (int num : nums) { freq[num]++; }

    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> heap; // {cnt, val}, top by min cnt
    for (const auto& [val, cnt] : freq) {
        heap.push({cnt, val});
        if (static_cast<int>(heap.size()) > k) { heap.pop(); }
    }

    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// bucket sort, T: O(n), S: O(n)
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) { freq[num]++; }
    const int n = nums.size();
    vector<vector<int>> buckets(n + 1); // buckets[cnt] = [val,]
    for (const auto& [val, cnt] : freq) {
        buckets[cnt].push_back(val);
    }
    vector<int> result;
    for (int cnt = n; cnt >= 1; cnt--) {
        for (int val : buckets[cnt]) {
            result.push_back(val);
            if (static_cast<int>(result.size()) == k) { return result; }
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
              sorted(topKFrequentBucket(nums, k)) == sorted(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 1, 2, 2, 2, 3}, 2, {1, 2});
    test("all", {1, 2}, 2, {1, 2});
    test("neg", {-1, -1, 2}, 1, {-1});
}
