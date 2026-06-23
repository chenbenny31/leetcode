// state-machine dp, T: O(nk), S: O(k)

#include <vector>
#include <climits> // INT_MIN
#include <algorithm> // std::min

class Solution {
public:
    int maxProfit(int k, std::vector<int>& prices) {
        int n = static_cast<int>(prices.size());

        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++) {
                profit += std::max(0, prices[i] - prices[i-1]);
            }
            return profit;
        }

        std::vector<int> hold(k, INT_MIN); // hold[i] = max profit when free for i-th buy
        std::vector<int> cash(k, 0); // cash[i] = max profit when completed i-th buy

        for (int p : prices) {
            for (int i = k - 1; i >= 0; i--) {
                cash[i] = std::max(cash[i], hold[i] + p);
                hold[i] = std::max(hold[i], (i > 0 ? cash[i-1] : 0) - p);
            }
        }
        return cash[k-1];
    }
};

// k >= n/2: at most n/2 transactions possible in n days
// reverse inner loop i = k-1 to 0: cash[i] <- hold[i] <- cash[i-1] <- hold[i-1], prevent correct day-chaining
// i > 0 ? cash[i-1] : 0: first transaction byf from 0 capital, subsequent from cash[i-1]
