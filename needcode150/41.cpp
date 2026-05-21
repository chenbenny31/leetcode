// index-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs
#include <algorithm> // std::max

class Solution {
public:
    int firstMissingPositive(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());

        for (int& x : nums) {
            if (x <= 0 || x > n) { x = n + 1; }
        }
        
        for (int i = 0; i < n; i++) {
            int val = std::abs(nums[i]);
            if (val <= n) { nums[val - 1] = -std::abs(nums[val - 1]); }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) { return i + 1; }
        }
        return n + 1;
    }
};

// cycle-sort, first index where nums[i] != i + 1 is the answer, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int firstMissingPositive(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int i = 0;

        while (i < n) {
            if (nums[i] > 0 && nums[i] <= n) {
                int correct = nums[i] - 1; // need bound check first
                if (nums[i] != nums[correct]) { std::swap(nums[i], nums[correct]); }
                else { i++; }
            } else {
                ++i;
            }
        }

        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) { return i + 1; }
        } 
        return n + 1;
    }
};

// answer must be in [1, n+1]: pigeonhole, worst case is [1, 2..n], answer is n+1
// clamp to n+1 not 0 or -1: n+1 is out of valid range
// -std::abs(nums[val - 1]) not -nums[val - 1]: 
