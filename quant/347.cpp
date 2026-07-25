// min-heap

#include <queue>
#include <unordered_map>
#include <utility> // std::pair
#include <algorithm>

using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::greater;
using std::vector;

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        unordered_map<int, int> freq; // nums[i]: its-count
        freq.reserve(n);
        freq.max_load_factor(0.25f);
        for (int i = 0; i < n; i++) {
            freq[nums[i]]++;
        }

        vector<pair<int, int>> buf;
        buf.reserve(k + 1);

        priority_queue<pair<int, int>, vector<pair<int, int>>,
                       greater<pair<int, int>>> minHeap(greater<pair<int, int>>{}, std::move(buf));
        for (auto& [val, cnt] : freq) {
            minHeap.push({cnt, val});
            if (static_cast<int>(minHeap.size()) > k) { minHeap.pop(); }
        }

        vector<int> out(k);
        for (int i = k - 1; i >= 0; i--) {
            out[i] = minHeap.top().second;
            minHeap.pop();
        }
        return out;
    }
};

// bucket-sort

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        unordered_map<int, int> freq; // nums[i]: its-count
        freq.reserve(n);
        freq.max_load_factor(0.25f);
        for (int num : nums) {
            freq[num]++;
        }

        vector<vector<int>> buckets(n + 1); // buckets[freq] = [vals,]
        for (auto& [val, cnt] : freq) {
            buckets[cnt].push_back(val);
        }

        vector<int> out;
        out.reserve(k);
        for (int i = n; i >= 1 && static_cast<int>(out.size()) < k; i--) {
            for (int v : buckets[i]) {
                out.push_back(v);
                if (static_cast<int>(out.size()) == k) { break; }
            }
        }
        return out;
    }
};

// quick-select

#include <random> // std::mt19937, std::random_device
#include <algorithm> // std::shuffle
#include <utility> // std::swap, std::pair

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        freq.reserve(nums.size());
        freq.max_load_factor(0.25f);
        for (int num : nums) { freq[num]++; }

        vector<pair<int, int>> entries;
        entries.reserve(freq.size());
        for (auto& [val, cnt] : freq) { entries.push_back({cnt, val}); }

        mt19937 rng(random_device{}());
        shuffle(entries.begin(), entries.end(), rng);

        int n = static_cast<int>(entries.size());
        int target = n - k;
        int lo = 0;
        int hi = n - 1;
        while (lo < hi) {
            auto [lt, gt] = partition3(entries, lo, hi);
            if (target > gt) { lo = gt + 1; }
            else if (target < lt) { hi = lt - 1; }
            else { break; }
        }

        vector<int> out;
        out.reserve(k);
        for (int i = target; i < n; i++) { out.push_back(entries[i].second); }
        return out;
    }

private:
    pair<int, int> partition3(vector<pair<int, int>>& entries, int lo, int hi) {
        int pivot = entries[lo].first;
        int lt = lo;
        int gt = hi;
        int i = lo;
        while (i <= gt) {
            if (entries[i].first < pivot) { swap(entries[i++], entries[lt++]); }
            else if (entries[i].first > pivot) { swap(entries[i], entries[gt--]); }
            else { i++; }
        }
        return {lt, gt};
    }
};
