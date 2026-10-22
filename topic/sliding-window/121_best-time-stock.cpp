// one pass state tracing, Kadane's Algorithm, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::min
#include <climits> // INT_MAX

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int minPrice = INT_MAX;
        int maxProfit = 0;

        for (int price : prices) {
            maxProfit = std::max(maxProfit, price - minPrice);
            minPrice = std::min(minPrice, price);
        }
        return maxProfit;
    }
};

// state-machine, T: O(n), S: O(1)

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int hold = -prices[0]; // buy on day0 -> become hold state
        int cash = 0; // never transacted -> become cash state

        for (int price : prices) {
            // case need yesterday's hold to update first
            cash = std::max(cash, hold + price); // unchange or sell today
            hold = std::max(hold, -price); // unchange or buy today (only one buy)
        }
        return cash;
    }
};
