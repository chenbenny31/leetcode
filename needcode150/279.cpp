// dp, T: O(n*sqrt(n)), S: O(n)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int numSquares(int n) {
        std::vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j * j <= i; j++) {
                if (dp[i - j*j] != INT_MAX) {
                    dp[i] = std::min(dp[i], dp[i - j*j] + 1);
                }
            }
        }
        return dp[n];
    }
};

// bfs, T: O(n*sqrt(n)), S: O(n)

#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    int numSquares(int n) {
        std::vector<uint8_t> visited(n + 1, 0);
        std::vector<int> q;
        q.reserve(n + 1);
        int head = 0;

        q.push_back(0);
        visited[0] = 1;
        int steps = 0;

        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;
            steps++;

            for (int i = 0; i < sz; i++) {
                int curr = q[head];
                head++;

                for (int j = 1; curr + j*j <= n; j++) {
                    int next = curr + j*j;
                    if (next == n) { return steps; }
                    if (!visited[next]) {
                        visited[next] = 1;
                        q.push_back(next);
                    }
                }
            }
        }
        return steps;
    }
};

// bfs level: number of squares used
