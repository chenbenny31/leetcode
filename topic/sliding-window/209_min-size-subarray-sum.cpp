// sliding-window, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        int left = 0;
        int sum = 0;
        int len = INT_MAX;

        for (int right = 0; right < static_cast<int>(nums.size()); right++) {
            sum += nums[right];
            while (sum >= target) { // all vals positive, shrinking only lowers the sum
                len = std::min(len, right - left + 1);
                sum -= nums[left];
                left++;
            }
        }
        return len == INT_MAX ? 0 : len;
    }
};

// prefix-sum + binary-search when all values positive, T: O(nlogn), S: O(n)
// follow-up: the required O(nlogn) solution

class Solution {
public:
    int minSubArrayLen(int target, std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        std::vector<long long> prefix(n + 1, 0); // all vals postive: prefix is strictly incre -> enable binary-search
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        int len = INT_MAX;
        for (int i = 0; i < n; i++) {
            long long need = static_cast<long long>(target) + prefix[i]; // search need in [i+1,n)
            int lo = i + 1;
            int hi = n;
            while (lo < hi) { // lower_bound: first prefix >= need
                int mid = lo + (hi - lo) / 2;
                if (prefix[mid] < need) {
                    lo = mid + 1;
                } else {
                    hi = mid;
                }
            }

            if (lo <= n && prefix[lo] >= need) { len = std::min(len, lo - i); }
        }
        return len == INT_MAX ? 0 : len;
    }
};
