// LC 122 Best Time to Buy and Sell Stock II: prices[i] is the price on day i. You
// may buy and sell many times but hold at most one share. Max profit?
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// Two-state machine (cash / hold) updated day by day. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    int n = prices.size();
    // cash = best profit holding nothing, hold = best profit holding a share
    int cash = 0, hold = -prices[0];
    for (int i = 1; i < n; i++) {
        cash = max(cash, hold + prices[i]);
        hold = max(hold, cash - prices[i]);
    }
    return cash;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-trades", {1, 5, 3, 6}, 7);
    test("falling", {5, 4, 3}, 0);
    test("single-day", {7}, 0);
}
