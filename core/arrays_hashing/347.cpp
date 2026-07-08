// min-heap of size k. T: O(nlogk), S: O(n) + O(k)

#include <vector>
#include <unordered_map>
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <functional> // std::greater

class Solution {
public:
    std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
        int n = static_cast<int>(nums.size());
        std::unordered_map<int, int> freq;
        freq.reserve(n);
        freq.max_load_factor(0.25f);
        for (int i = 0; i < n; i++) { freq[nums[i]]++; }

        std::vector<std::pair<int, int>> buf;
        buf.reserve(k + 1);
        std::priority_queue<std::pair<int, int>,
                            std::vector<std::pair<int, int>>,
                            std::greater<std::pair<int, int>>> min_heap(std::greater<std::pair<int, int>>{}, std::move(buf));

        for (auto& [val, cnt] : freq) {
            min_heap.push({cnt, val});
            if (static_cast<int>(min_heap.size()) > k) { min_heap.pop(); }
        }

        std::vector<int> out(k);
        for (int i = k - 1; i >= 0; i--) {
            out[i] = min_heap.top().second;
            min_heap.pop();
        }
        return out;
    }
};

// bucket-sort, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>

class Solution {
public:
    std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());

        std::unordered_map<int, int> freq;
        freq.reserve(n);
        freq.max_load_factor(0.25f);
        for (int i = 0; i < n; i++) { freq[nums[i]]++; }

        std::vector<std::vector<int>> buckets(n + 1);
        for (auto& [val, cnt] : freq) {
            buckets[cnt].push_back(val);
        }

        std::vector<int> out;
        out.reserve(k);
        for (int i = n; i >= 1 && static_cast<int>(out.size()) < k; i--) {
            for (int val : buckets[i]) {
                out.push_back(val);
                if (static_cast<int>(out.size()) == k) { break; }
            }
        }
        return out;
    }
};

// quick-sort, 3-way partition + std::mt19937 shuffle, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>
#include <random> // std::mt19937, std::random_device
#include <algorithm> // std::shuffle
#include <utility> // std::pair, std::swap

class Solution {
private:
    std::pair<int, int> partition3(std::vector<std::pair<int, int>>& entries, int lo, int hi) {
        int pivot = entries[lo].first;
        int lt = lo;
        int gt = hi;
        int i = lo;
        while (i <= gt) {
            if (entries[i].first < pivot) { std::swap(entries[i++], entries[lt++]); }
            else if (entries[i].first > pivot) { std::swap(entries[i], entries[gt--]); }
            else { i++; }
        }
        return {lt, gt};
    }

public:
    std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> freq;
        freq.reserve(nums.size());
        freq.max_load_factor(0.25f);
        for (int val : nums) { freq[val]++; }

        std::vector<std::pair<int, int>> entries;
        entries.reserve(freq.size());
        for (auto& [val, cnt] : freq) { entries.push_back({cnt, val}); }

        std::mt19937 rng(std::random_device{}());
        std::shuffle(entries.begin(), entries.end(), rng);

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

        std::vector<int> out;
        out.reserve(k);
        for (int i = target; i < n; i++) { out.push_back(entries[i].second); }
        return out;
    }
};

// shuffle is necessary: avoid degrading to O(n2) with already sorted input
// 3-way partition: handles highly repetitive count nums
// bucket-sort space doesn't scale: always n+1
