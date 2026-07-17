// sliding-window, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        int left = 0;
        int sum = 0;
        int bestLen = INT_MAX;

        for (int right = 0; right < static_cast<int>(nums.size()); right++) {
            sum += nums[right];

            while (sum >= target) {
                bestLen = std::min(bestLen, right - left + 1);
                sum -= nums[left];
                left++;
            }
        }
        return bestLen == INT_MAX ? 0 : bestLen;
    }
};

// prefix-sum + binary-search when all values positive, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        std::vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) { prefix[i + 1] = prefix[i] + nums[i]; }

        int bestLen = INT_MAX;
        for (int i = 0; i < n; i++) {
            long long need = static_cast<long long>(target) + prefix[i];
            int lo = i + 1;
            int hi = n;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                if (prefix[mid] < need) { lo = mid + 1; }
                else { hi = mid; }
            }

            if (lo <= n && prefix[lo] >= need) { bestLen = std::min(bestLen, lo - i); }
        }
        return bestLen == INT_MAX ? 0 : bestLen;
    }
};

// prefix[lo] >= need check: avoid lo land on n via empty loop, indistinguishable from lo == n meaning found a match
