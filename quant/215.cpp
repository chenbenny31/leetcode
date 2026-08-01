// quick-select

#include <vector>
#include <algorithm> // std::swap, std::shuffle
#include <random> // std::mt19937, std::random_device
#include <utility> // std::pair

class Solution {
private:
    std::pair<int, int> partition3(std::vector<int>& nums, int lo, int hi) {
        int pivot = nums[lo];
        int lt = lo;
        int gt = hi;
        int i = lo;
        while (i <= gt) {
            if (nums[i] < pivot) { std::swap(nums[i++], nums[lt++]); }
            else if (nums[i] > pivot) { std::swap(nums[i], nums[gt--]); }
            else { i++; }
        }
        return {lt, gt};
    }

public:
    int findKthLargest(std::vector<int>& nums, int k) {
        const int n = static_cast<int>(nums.size());
        std::mt19937 rng(std::random_device{}());
        std::shuffle(nums.begin(), nums.end(), rng);

        int target = n - k;
        int lo = 0;
        int hi = n - 1;

        while (lo < hi) {
            auto [lt, gt] = partition3(nums, lo, hi);
            if (target < lt) { hi = lt - 1; }
            else if (target > gt) { lo = gt + 1; }
            else { break; }
        }
        return nums[target];
    }
};

// counting-sort

class Solution {
public:
    int findKthLargest(std::vector<int>& nums, int k) {
        constexpr int OFFSET = 10000; // shift[-1e4, 1e4] into [0, 2e4]
        constexpr int R = 20001;

        std::vector<int> count(R, 0);
        for (int num : nums) {
            count[num + OFFSET]++;
        }

        int remain = k;
        for (int v = R - 1; v >= 0; v--) {
            remain -= count[v];
            if (remain <= 0) {
                return v - OFFSET;
            }
        }
        return -1;
    }
};
