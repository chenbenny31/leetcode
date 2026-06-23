// state machine dp, T: O(n), S: O(1)

#include <vector>
#include <climits> // INT_MIN

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int hold1 = INT_MIN, cash1 = 0;
        int hold2 = INT_MIN, cash2 = 0;

        for (int p : prices) {
            cash2 = std::max(cash2, hold2 + p); // complete 2nd buy
            hold2 = std::max(hold2, cash1 - p); // free for 2nd buy
            cash1 = std::max(cash1, hold1 + p); // complete 1st buy
            hold1 = std::max(hold1, -p); // free for 1st buy
        }
        return cash2;
    }
};

// prefix/suffix dp, T: O(n), S: O(n)

#include <vector>
#include <algorithm> // std::max
#include <climits> // INT_MIN

class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int n = static_cast<int>(prices.size());
        std::vector<int> left(n, 0), right(n, 0);

        int minPrice = prices[0];
        for (int i = 1; i < n; i++) {
            minPrice = std::min(minPrice, prices[i]);
            left[i] = std::max(left[i-1], prices[i] - minPrice);
        }

        int maxPrice = prices[n-1];
        for (int i = n - 2; i >= 0; i--) {
            maxPrice = std::max(maxPrice, prices[i]);
            right[i] = std::max(right[i+1], maxPrice - prices[i]);
        }

        int best = 0;
        for (int i = 0; i < n; i++) {
            best = std::max(best, left[i] + right[i]);
        }
        return best;
    }
};

// state-machine update order: cash2 <- hold2 <- cash1 <- hold1
// prefix/suffic split point: split at every possible point, take max from left[i], right[i]
