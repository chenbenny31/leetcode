// LC 121 Best Time to Buy and Sell Stock (Easy). Pattern: running min, one pass.
// prices[i] = price on day i; buy once, then sell on a later day. Return the max
// profit, 0 if no profit is possible.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// Sell today against the cheapest earlier day. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    // cheapest price seen so far
    int minPrice = prices[0];
    int best = 0;
    for (int price : prices) {
        best = max(best, price - minPrice);
        minPrice = min(minPrice, price);
    }
    return best;
}

// LC 122: add every positive day-to-day rise. T: O(n), S: O(1).
int maxProfitUnlimited(const vector<int>& prices) {
    int total = 0;
    int n = prices.size();
    for (int i = 1; i < n; i++) {
        // buy yesterday, sell today only when it gains; this sums every upswing
        total += max(0, prices[i] - prices[i - 1]);
    }
    return total;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

void testUnlimited(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfitUnlimited(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {3, 1, 4, 2}, 3);
    test("late-min", {2, 5, 1, 3}, 3);
    test("falling", {3, 2, 1}, 0);
    test("one-day", {5}, 0);
    testUnlimited("two-rises", {1, 3, 2, 5}, 5);
    testUnlimited("steady-climb", {1, 2, 3, 4}, 3);
    testUnlimited("all-falling", {4, 3, 1}, 0);
}

// follow-up: at most k transactions (LC 188) -> dp states buy[j] / sell[j] per j <= k,
//   O(n * k); cooldown variant is LC 309
