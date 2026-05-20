// hash-set, T: O(n), S: O(n)

#include <vector>
#include <unordered_set>

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::unordered_set<int> seen(nums.begin(), nums.end());
        for (int i = 0; i <= n; i++) {
            if (!seen.count(i)) { return i; }
        }
        return -1;
    }
};

// flat-array, bounded range n <= 10^4, T: O(n), S: O(n) stack-allocated

#include <vector>
#include <cstring>

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        constexpr int R = 10001;
        bool seen[R];
        std::memset(seen, 0, sizeof(seen));

        for (int x : nums) { seen[x] = true; }

        for (int i = 0; i <= static_cast<int>(nums.size()); i++) {
            if (!seen[i]) { return i; }
        }
        return -1;
    }
};

// gauss sum/XOR, expected sum - actual sum = missing val, XOR avoids overflow, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int missingNumber(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());

        /* gauss expected sum = n * (n + 1) / 2;
        int expect = n * (n + 1) / 2;
        int actual = 0;
        for (int x : nums) { actual += x; }
        return expected - actual;
        */

        // XOR
        int res = n;
        for (int i = 0; i < n; i ++) { res ^= i ^ nums[i]; }
        return res;
    }
};

// Gauss vs XOR: XOR has no overflow risk
// XOR self-cancellation: every val in [0..n] cancels to 0, missing val has no pair
// two missing nums: two eqn or flat-array
