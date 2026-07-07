// sorting, T: O(nlogn), S: O(1) for in-place, O(logn) recur stack from std::sort
// hash-set, T: O(n), S: O(n)
// cycle-sort with boundary-adjust for n T: O(n), S: O(1)

// sum formula, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        int expect_sum = n * (n + 1) / 2;
        int actual_sum = 0;
        for (int i = 0; i < n; i++) { actual_sum += nums[i]; }

        return expect_sum - actual_sum;
    }
};

// XOR trick, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        int res = n; // in [0..n] with no loop-idx counterpart
        for (int i = 0; i < n; i++) {
            res ^= i;
            res ^= nums[i];
        }
        return res;
    }
};

// order of sum formula matters: n*(n+1) is always even, div 2 is safe in int
// XOR overflow immunity: XOR neve produces value outside the operand width
// XOR correctness: all num in [0..n] has x^x = 0, the net res is 0^missing_x = missing_x
// XOR not for 41: XOR need all elem uniq, otherwise dup elem break the parity
