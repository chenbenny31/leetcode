// unbounded knapsack dp, T: O(n * amount), S: O(amount) 

#include <vector>

class Solution {
public:
    int change(int amount, std::vector<int>& coins) {
        std::vector<unsigned int> dp(amount + 1, 0);
        dp[0] = 1; // one way to make 0, empty combination
        
        for (int c : coins) {
            for (int j = c; j <= amount; j++) {
                dp[j] += dp[j-c];
            }
        }
        return static_cast<int>(dp[amount]);
    }
};

// outer loop on coins: combinations not permutation
// forward inner loop: unbounded reuse
