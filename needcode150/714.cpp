// state-machine dp, T: O(n), S: O(1)

#include <vector>
#include <climits> // LLONG_MIN
#include <algorithm> // std::max

class Solution {
public:
    int maxProfit(std::vector<int>& prices, int fee) {
        long long hold = LLONG_MIN / 2; // safe sentinel, room for subtraction
        long long cash = 0;

        for (int p : prices) {
            long long prev_cash = cash;
            cash = std::max(cash, hold + p - fee);
            hold = std::max(hold, prev_cash - p);
        }
        return static_cast<int>(cash);
    }
};

// prev_cash is needed here: capture yesterday's cash before today's mutation, otherwise hold uses today's cash, needed when allowing sameday sell->buy
