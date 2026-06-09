// min-heap of size k, T: O(nlogk), S: O(k)

#include <vector>
#include <queue> // priority_queue
#include <functional> // std::greater

class Solution {
public:
    int findKthLargest(std::vector<int>& nums, int k) {
        std::vector<int> buf;
        buf.reserve(k + 1);
        std::priority_queue<int, std::vector<int>, std::greater<int>> heap(std::greater<int>{}, std::move(buf)); // min_heap

        for (int x : nums) {
            heap.push(x);
            if (static_cast<int>(heap.size()) > k) { heap.pop(); }
        }
        return heap.top();
    }
};

// quick-select with 3-way partition, T: O(n) avg, S: O(1)

#include <vector>
#include <algorithm> // std::swap
#include <random> // std::mt19937 
#include <utility> // std::pair

class Solution{
public:
    int findKthLargest(std::vector<int>& nums, int k) {
        std::mt19937 rng(std::random_device{}());
        std::shuffle(nums.begin(), nums.end(), rng);

        int target = static_cast<int>(nums.size()) - k;
        int lo = 0, hi = static_cast<int>(nums.size()) - 1;

        while (lo < hi) {
            auto [lt, gt] = partition(nums, lo, hi);
            if (lt > target) { hi = lt - 1; }
            else if (gt < target) { lo = gt + 1; }
            else { break; }
        }
        return nums[target];
    }

private:
    std::pair<int, int> partition(std::vector<int>& nums, int lo, int hi) {
        int pivot = nums[lo];
        int lt = lo, gt = hi, i = lo;
        while (i <= gt) {
            if (nums[i] < pivot) { std::swap(nums[i++], nums[lt++]); }
            else if (nums[i] > pivot) { std::swap(nums[i], nums[gt--]); }
            else { i++; }
        }
        return {lt, gt};
    }
};

// counting-sort with bound data, T: O(n + R), R = value range, S: O(R)

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    int findKthLargest(std::vector<int>& nums, int k) {
        constexpr int BASE = -10000;
        constexpr int R = 20001;

        std::vector<int> cnt(R, 0);
        for (int x : nums) { cnt[x - BASE]++; }

        for (int i = R - 1; i >= 0; i--) {
            k -= cnt[i];
            if (k <= 0) { return i + BASE; }
        }
        return -1;
    }
};

// three approaches win: min-heap best for streaming or k << n; counting-sort small R; quick-select best for k ~= n/2
// couting-sort cnt[R] = 80KB fits in L2
// 3-way partition: 3-way partition skips duplicate values
