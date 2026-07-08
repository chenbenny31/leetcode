// sort, T: O(nlogn), S: O(1)
// hash-set: T: O(n), S: O(n)
// binary-search on value range, T: O(nlogn), S: O(1)

// Floyd's Cycle Detection. T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int slow = 0;
        int fast = 0;

        do {
            slow = nums[slow];
            fast = nums[nums[fast]];
        } while (slow != fast);

        slow = 0;
        while (slow != fast) {
            slow = nums[slow];
            fast = nums[fast];
        }
        return slow;
    }
};

// negative marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int res = -1;
        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]);
            if (nums[idx] < 0) { res = idx; break; }
            nums[idx] *= -1;
        }

        // restore
        for (int i = 0; i < n; i++) { nums[i] = std::abs(nums[i]); }

        return res;
    }
};

// do-while loop: prevent first same entry of slow/fast to stop
