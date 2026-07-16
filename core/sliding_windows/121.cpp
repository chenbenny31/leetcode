// Kadane's Algorithm, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int curr = 0;
        int best = 0;

        for (int i = 1; i < static_cast<int>(prices.size()); i++) {
            int diff = prices[i] - prices[i - 1];
            curr = std::max(diff, curr + diff);
            best = std::max(best, curr);
        }
        return best;
    }
};

// state-machine, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int hold = -prices[0];
        int cash = 0;

        for (int i = 1; i < static_cast<int>(prices.size()); i++) {
            int prevHold = hold;
            hold = std::max(hold, -prices[i]); // unchange or buy today
            cash = std::max(cash, prevHold + prices[i]); // unchange or sell today
        }
        return cash;
    }
};

// hold/cash is state at end of today: hold = best profit when holding a share; cash = best profit when holding nothing
// prevHold: preserve unchange state of hold as cash updating based on it
