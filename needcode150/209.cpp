// sliding-window, T: O(n), S: O(1)

#include <vector>
#include <climits> // INT_MAX

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        int left = 0;
        int sum = 0;
        int best = INT_MAX;

        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            sum += nums[right];
            while (sum >= target) {
                best = std::min(best, right - left + 1);
                sum -= nums[left++];
            }
        }
        return best == INT_MAX ? 0 : best;
    }
};

// prefix-sum + binary-search, T: O(nlogn), S: O(n)

#include <vector>
#include <numeric> // std::accumulate
#include <climits> // INT_MAX
#include <algorithm> // std::upper_bound, std::min

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        int best = INT_MAX;
        for (int i = 1; i <= n; i++) {
            int need = prefix[i] - target;
            auto it = std::upper_bound(prefix.begin(), prefix.begin() + i, need);
            if (it != prefix.begin()) {
                it--; // largest j with prefix[j] <= need
                best = std::min(best, i - static_cast<int>(it - prefix.begin()));
            }
        }
        return best == INT_MAX ? 0 : best;
    }
};

// sliding-window requires all pos vals: neg val break mono prop
// upper_bound + --it: find first prefix > need, step back one - largest prefix <= need
// cache sliding-window single seq pass with optimal pre-fetcher behavior
// while not if: window might need multiple shrink times before sum < target
// sliding-window vs prefix-sum + binary-search: only when with neg vals
