// dp

class Solution {
public:
    int change(int amount, vector<int>& coins) {
        const int n = static_cast<int>(coins.size());
        std::vector<unsigned int> dp(amount + 1, 0);
        dp[0] = 1;

        for (int coin : coins) {
            for (int i = coin; i <= amount; i++) {
                dp[i] += dp[i - coin];
            }
        }
        return static_cast<int>(dp[amount]);
    }
};
