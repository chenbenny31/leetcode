// dp, T: O(n), S: O(n)

#include <string>
#include <vector>

class Solution {
public:
    int numDecodings(std::string s) {
        int n = static_cast<int>(s.size());
        std::vector<int> dp(n + 1, 0);
        dp[0] = 1; // empty string for one way
        dp[1] = (s[0] != '0') ? 1 : 0;

        for (int i = 2; i <= n; i++) {
            int one = s[i-1] - '0'; // single digit
            int two = (s[i-2] - '0') * 10 + (s[i-1] - '0'); // double digits
            if (one >= 1 && one <= 9) { dp[i] += dp[i-1]; }
            if (two >= 10 && two <= 26) { dp[i] += dp[i-2]; }
        }
        return dp[n];
    }
};

// two variables, T: O(n), S: O(1)

#include <string>

class Solution {
public:
    int numDecodings(std::string s) {
        int n = static_cast<int>(s.size());
        int prev2 = 1;
        int prev1 = (s[0] != '0') ? 1 : 0;

        for (int i = 2; i <= n; i++) {
            int curr = 0;
            int one = s[i-1] - '0';
            int two = (s[i-2] - '0') * 10 + (s[i-1] - '0');
            if (one >= 1 && one <= 9) { curr += prev1; }
            if (two >= 10 && two <= 26) { curr += prev2; }
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};

// dp[0] = 1: empty string sentinel, dp[i] += dp[i-2] needs dp[0] when i=2, empty string has one decoding way
