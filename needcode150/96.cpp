// dp, T: O(n2), S: O(n)

#include <vector>

class Solution {
public:
    int numTrees(int n) {
        std::vector<int> dp(n + 1, 0); // dp[i] = num of uniq bst formed with exact i nodes
        dp[0] = 1; // empty tree, one bst
        dp[1] = 1; // single node, one bst

        for (int i = 2; i <= n; i++) {
            for (int j = 1; j <= i; j++) {
                dp[i] += dp[j-1] * dp[i-j];
            }
        }
        return dp[n];
    }
};

// catalan number, T: O(n), S: O91)

#include <cstdint>

class Solution {
public:
    int numTrees(int n) {
        int64_t res = 1;
        for (int i = 0; i < n; i++) {
            res = res * 2 * (2*i + 1) / (i + 2);
        }
        return static_cast<int>(res);
    }
};

// dp[i] = sum(dp[j-1] * dp[i-j]) for j = 1..i: root j splits into left subtree of j-1 nodes and right subtree of i-j nodes
// catalan number C(n) = C(2n,n)/(n+1), iterative computation via res * 2*(2i+1)/(i+2)
