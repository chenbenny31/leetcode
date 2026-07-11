// hash-set, T: O(n), S: O(n)

// sign-marking, T: O(n), S: O(1)

#include <vector>
#include <cstdlib> // std::abs

class Solution {
public:
    std::vector<int> findErrorNums(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        int dup = -1;

        for (int i = 0; i < n; i++) {
            int idx = std::abs(nums[i]) - 1;
            if (nums[idx] < 0) {
                dup = std::abs(nums[i]);
            } else {
                nums[idx] *= -1;
            }
        }

        int miss = -1;
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) {
                miss = i + 1;
            }
        }
        return {dup, miss};
    }
};

// XOR partition, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    std::vector<int> findErrorNums(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());

        int xs = 0;
        for (int i = 1; i <=n; i++) {
            xs ^= i;
            xs ^= nums[i - 1];
        }

        int lb = xs & (-xs); // lowerset set bit

        int a = 0;
        for (int i = 1; i <= n; i++) {
            if (i & lb) {
                a ^= i;
            }
            if (nums[i - 1] & lb) {
                a ^= nums[i - 1];
            }
        }

        int b = xs ^ a;
        for (int i = 0; i < n; i++) {
            if (nums[i] == a) {
                return {a, b};
            }
        }
        return {b, a};
    }
};

// why not Floyd's transfer: need guarantee pigeonhold to force a cycle
// XOR-partition: isolate two unq val via combimed XOR, then split by their lowest diff bit, nums[i] == a to check which one is dup/miss
