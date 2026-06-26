// 2d-dp, T: O(mn), S: O(mn)

#include <vector>
#include <algorithm> // std::min, std::max

class Solution {
public:
    int maximalSquare(std::vector<std::vector<char>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        int best = 0;

        for (int r = 1; r <= m; r++) {
            for (int c = 1; c <= n; c++) {
                if (matrix[r-1][c-1] == '1') {
                    dp[r][c] = 1 + std::min({dp[r-1][c], dp[r][c-1], dp[r-1][c-1]});
                    best = std::max(best, dp[r][c]);
                }
            }
        }
        return best * best;
    }
};

// 1d-dp, T: O(mn), S: O(n)

#include <vector>
#include <algorithm> // std::min, std::max

class Solution {
public:
    int maximalSquare(std::vector<std::vector<char>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        std::vector<int> dp(n + 1, 0);
        int best = 0;
        int diag = 0;

        for (int r = 1; r <= m; r++) {
            for (int c = 1; c <= n; c++) {
                int above = dp[c]; // save dp[r-1][c]
                if (matrix[r-1][c-1] == '1') {
                    dp[c] = 1 + std::min({dp[c], // dp[r-1][c]
                                          dp[c-1], // dp[r][c-1]
                                          diag}); // dp[r-1][c-1]
                    best = std::max(best, dp[c]);
                } else {
                    dp[c] = 0;
                }
                diag = above;
            }
            diag = 0; // reset diag for new row
        }
        return best * best;
    }
};

// dp[r][c] = min(above, left, diag) + 1: square must extend above, left and diagonally
