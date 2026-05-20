// flat-array, T: O(n), S: O(n) stack-alloc

#include <vector>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<int> findErrorNums(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        constexpr int R = 10'001;
        int freq[R];
        std::memset(freq, 0, sizeof(freq));

        for (int x : nums) { freq[x]++; }

        int dup = -1, mis = -1;
        for (int i = 1; i <= n; i++) {
            if (freq[i] == 2) { dup = i; }
            if (freq[i] == 0) { mis = i; }
            if (dup != -1 && mis != -1) { break; }
        }
        return {dup, mis};
    }
};

// index-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findErrorNums(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        int dup = -1, mis = -1;

        for (int x : nums) {
            int idx = std::abs(x) - 1;
            if (nums[idx] < 0) { dup = std::abs(x); }
            else { nums[idx] = -nums[idx]; }
        }
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {
                mis = i + 1;
                break;
            }
        }
        return {dup, mis};
    }
};

// index-marking with early exit
// flat-array: freq[10001], ~40KB, fits in L1
// follow-up: math with long long, need sum and square-sum
