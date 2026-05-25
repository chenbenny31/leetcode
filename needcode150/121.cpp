// one-pass, track-extremity, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::min, std::max
#include <climits> // INT_MAX

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int min_price = INT_MAX;
        int best = 0;

        for (int price : prices) {
            min_price = std::min(min_price, price);
            best = std::max(best, price - min_price);
        }
        return best;
    }
};


// best = 0 not INT_MIN: can choose not to trade
// Kadane's conn: max sub arr sum on diff = max single transaction profit
// std::min, std::max are branchless, compile to cmov on x86
