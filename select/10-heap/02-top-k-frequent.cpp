// LC 347 Top K Frequent Elements (Medium). Pattern: count + size-k min-heap / buckets.
// Return the k most frequent values of nums, in any order. The answer is unique.

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
using std::priority_queue;
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
    test("top-two", {4, 4, 4, 6, 6, 9}, 2, {4, 6});
    test("all", {3, 8}, 2, {3, 8});
    test("negative", {-2, -2, 5}, 1, {-2});
}

// follow-up: k-th most frequent words in lexicographic order (LC 692) -> heap of
//   (-count, word) or a comparator that breaks count ties by word
// follow-up: stream -> keep freq map + heap; or quickselect on (count, value), O(n) avg
