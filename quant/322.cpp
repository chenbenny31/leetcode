// dp

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int coinChange(std::vector<int>& coins, int amount) {
        std::vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i < amount; i++) {
            for (int coin : coins) {
                if (i - coin >= 0 && dp[i - coin] != INT_MAX) {
                    dp[i] = std::min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};

// bfs

#include <cstdint>

class Solution {
public:
    int coinChange(std::vector<int>& coins, int amount) {
        if (amount == 0) { return 0; }

        std::vector<int> q;
        q.reserve(amount + 1);
        int head = 0;

        q.push_back(0);
        std::vector<uint8_t> visited(amount + 1, 0);
        visited[0] = 1;

        int steps = 0;
        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;
            steps++;
            for (int i = 0; i < sz; i++) {
                int curr = q[head];
                head++;
                for (int coin : coins) {
                    int next = curr + coin;
                    if (next == amount) { return steps; }
                    if (next < amount && !visited[next]) {
                        visited[next] = 1;
                        q.push_back(next);
                    }
                }
            }
        }
        return -1;
    }
};
