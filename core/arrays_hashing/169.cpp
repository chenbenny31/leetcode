// hash-map freq counting
// sorting

// Boyer-Moore Voting, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int majorityElement(std::vector<int>& nums) {
        int cand = nums[0];
        int count = 0;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (count == 0) { cand = nums[i]; }
            count += (nums[i] == cand) ? 1 : -1;
        }
        return cand;
    }
};

// bit manipulation, T: O(32n) = O(n), S: O(1)

#include <vector>
#include <cstdint> // uint32_t

class Solution {
public:
    int majorityElement(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        constexpr int WIDTH = 32;

        uint32_t res = 0;

        for (int b = 0; b < WIDTH; b++) {
            uint32_t mask = 1u << b;
            int bit_cnt = 0;

            for (int i = 0; i < n; i++) {
                uint32_t bits = static_cast<uint32_t>(nums[i]);
                if (bits & mask) { bit_cnt++; }
            }

            if (bit_cnt > n / 2) { res |= mask; }
        }
        return static_cast<int>(res);
    }
};

// 1u << b: b is int and 1 << b is signed left-shift overflow, undef-behavior; unsigned int left shift safe
// static_cast<uint32_t>(nums[i]) -> bitwise AND -> static_cast<int>(res): 2's comple impl mostly
// Boyer-Morre: count cand's occurrence if need verify
// branch prediction: nums[i] == cand is data-dependent, predictor towards "matches", frendlier than a 50/50
