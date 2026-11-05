// ByteDance / LC 121: prices[i] = stock price on day i; one buy then one later sell.
// Return the max profit, 0 if no profit is possible.
// Follow-up LC 122: any number of transactions, holding at most one share at a time.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// Track the cheapest buy so far; sell today against it. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    // cheapest price on any earlier day
    int minPrice = prices[0];
    int best = 0;
    for (int p : prices) {
        best = max(best, p - minPrice);
        minPrice = min(minPrice, p);
    }
    return best;
}

// follow-up: collect every upward day-to-day move. T: O(n), S: O(1).
int maxProfitMulti(const vector<int>& prices) {
    int n = prices.size();
    int total = 0;
    for (int i = 1; i < n; i++) {
        total += max(0, prices[i] - prices[i - 1]);
    }
    return total;
}

void test(const string& label, const vector<int>& prices, int want, int wantMulti) {
    bool ok = maxProfit(prices) == want && maxProfitMulti(prices) == wantMulti;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {7, 1, 5, 3, 6, 4}, 5, 7);
    test("rising", {1, 2, 3, 4, 5}, 4, 4);
    test("falling", {7, 6, 4, 3, 1}, 0, 0);
    test("one-day", {5}, 0, 0);
}

// follow-up: unlimited transactions (LC 122) -> see maxProfitMulti() above; sum every
//   positive prices[i] - prices[i-1]
// follow-up: at most k transactions (LC 188) -> buy[j] = max(buy[j], sell[j-1] - p),
//   sell[j] = max(sell[j], buy[j] + p) for j = 1..k, O(n*k)
// follow-up: cooldown (LC 309) -> from old values: hold' = max(hold, rest - p),
//   sold' = hold + p, rest' = max(rest, sold); answer max(sold, rest), O(n)
