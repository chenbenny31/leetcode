// greedy, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int profit = 0;
        for (int i = 1; i < static_cast<int>(prices.size()); i++) {
            profit += std::max(0, prices[i] - prices[i-1]);
        }
        return profit;
    }
};

// dp, state-machine, T: O(n), S: O(1)

#include <algorithm> // std::max
#include <climits> // INT_MIN

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int hold = INT_MIN, cash = 0; // hold: currently holding a stock, cash: not holding free to buy
        for (int p : prices) {
            int prev_cash = cash;
            cash = std::max(cash, hold + p); // stay or sell stock now
            hold = std::max(hold, prev_cash - p); // stay or buy stock now with accumulated profit
        }
        return cash;
    }
};

// greedy correctness: summary all positive daily return capture all profitable trade without overlap <- sell and buy on the same day
// dp not greedy for state-machine: cash and hold represent max profit achievable in that state up to i
// hold initalizes to INT_MIN: ensures any hold + p becoming a valid buy
