// dp over reachable balances, T: O(n2), S: O(n)

#include <string>
#include <vector>
#include <cstdint> // uint8_t
#include <utility> // std::move

class Solution {
public:
    bool checkValidString(std::string s) {
        int n = static_cast<int>(s.size());
        std::vector<uint8_t> dp(n + 1, 0);
        dp[0] = 1;

        for (char c : s) {
            std::vector<uint8_t> nxt(n + 1, 0);
            for (int bal = 0; bal <= n; bal++) {
                if (!dp[bal]) { continue; }
                if (c == '(') {
                    if (bal + 1 <= n) { nxt[bal + 1] = 1; }
                } else if (c == ')') {
                    if (bal - 1 >= 0) { nxt[bal - 1] = 1; }
                } else {
                    if (bal + 1 <= n) { nxt[bal + 1] = 1; }
                    nxt[bal] = 1;
                    if (bal - 1 >= 0) { nxt[bal - 1] = 1; }
                }
            }
            dp = std::move(nxt);
        }
        return dp[0];
    }
};

// greedy interval tracking, T: O(n), S: O(1)

#include <string>
#include <algorithm> // std::max

class Solution {
public:
    bool checkValidString(std::string s) {
        int minBal = 0;
        int maxBal = 0;

        for (char c : s) {
            if (c == '(') {
                minBal++;
                maxBal++;
            } else if (c == ')') {
                minBal--;
                maxBal--;
            } else {
                minBal--; // '*' as ')'
                maxBal++; // '*' as '('
            }
            if (maxBal < 0) { return false; } // too many ')'
            minBal = std::max(minBal, 0); // discard assigned minBal
        }
        return minBal == 0;
    }
};

// reachable set is always contiguous: {v-1, v, v+1}
// minBal = max(minBal, 0): clamping, never force * to be ) if breaking validity
// maxBal < 0: * always ( still can't keep balance

