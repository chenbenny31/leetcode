// state-machine dp, T: O(n), S: O(1)

#include <vector>
#include <climits> // INT_MIN
#include <algorithm> // std::max

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int hold = INT_MIN;
        int cash = 0;
        int cool = 0;

        for (int p : prices) {
            int prev_cash = cash;
            cash = std::max(cash, hold + p);
            hold = std::max(hold, cool - p); // buy only from cooldown state
            cool = prev_cash; // cooldown = previous cash
        }
        return cash;
    }
};

// three states, cool is needed: without cooldown hold = max(hold, cash - p) buy from any cash state; with cooldown, cool = cash from two days ago
// cool = prev_cash: make cash one day delay, today's cool = yesterday's cash

