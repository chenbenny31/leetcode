// ByteDance / LC 347: return the k most frequent elements of nums, in any order. The
// answer is unique. Follow-up: better than O(n log n).
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

// Count, then keep a size-k min-heap of (count, value). T: O(n log k), S: O(n).
vector<int> topKFrequent(const vector<int>& nums, int k) {
    // freq[value] = occurrences in nums
    unordered_map<int, int> freq;
    for (int x : nums) { freq[x]++; }
    // min-heap of (count, value): top = least frequent kept candidate
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (pair<int, int> e : freq) {
        heap.push({e.second, e.first});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> result;
    while (!heap.empty()) {
        result.push_back(heap.top().second);
        heap.pop();
    }
    return result;
}

// follow-up: bucket by count (count <= n), scan buckets high to low. T: O(n), S: O(n).
vector<int> topKFrequentBucket(const vector<int>& nums, int k) {
    // freq[value] = occurrences in nums
    unordered_map<int, int> freq;
    for (int x : nums) { freq[x]++; }
    int n = nums.size();
    // buckets[c] = values occurring exactly c times
    vector<vector<int>> buckets(n + 1);
    for (pair<int, int> e : freq) { buckets[e.second].push_back(e.first); }
    vector<int> result;
    for (int c = n; c >= 1 && (int)result.size() < k; c--) {
        for (int val : buckets[c]) {
            if ((int)result.size() == k) { break; }
            result.push_back(val);
        }
    }
    return result;
}

void test(const string& label, const vector<int>& nums, int k, vector<int> want) {
    vector<int> got = topKFrequent(nums, k);
    vector<int> gotBucket = topKFrequentBucket(nums, k);
    sort(got.begin(), got.end());
    sort(gotBucket.begin(), gotBucket.end());
    sort(want.begin(), want.end());
    cout << label << (got == want && gotBucket == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {1, 1, 1, 2, 2, 3}, 2, {1, 2});
    test("single", {1}, 1, {1});
    test("negatives", {4, -1, -1, 4, 4, 2}, 1, {4});
}

// follow-up: O(n) time -> see topKFrequentBucket() above; buckets[count], high to low
// follow-up: quickselect -> nth_element on unique values by freq, avg O(n), worst O(n^2)
// follow-up: top k words (LC 692) -> min-heap top = worst (lower cnt, or same cnt and
//   larger word); pop into result and reverse, O(n log k)
