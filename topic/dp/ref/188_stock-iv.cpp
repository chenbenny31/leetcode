// LC 188 Best Time to Buy and Sell Stock IV: prices[i] is the price on day i. At
// most k transactions, one share held at a time. Max profit?
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// State machine per transaction count j. T: O(n*k), S: O(k).
int maxProfit(int k, const vector<int>& prices) {
    // buy[j] / sell[j] = best profit just after the j-th buy / sell; sell[0] = 0
    vector<int> buy(k + 1, -prices[0]), sell(k + 1, 0);
    for (int p : prices) {
        for (int j = 1; j <= k; j++) {
            buy[j] = max(buy[j], sell[j - 1] - p);
            sell[j] = max(sell[j], buy[j] + p);
        }
    }
    return sell[k];
}

void test(const string& label, int k, const vector<int>& prices, int want) {
    cout << label << (maxProfit(k, prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-trade", 1, {1, 5, 2, 9}, 8);
    test("two-trades", 2, {1, 5, 2, 9}, 11);
    test("zero-k", 0, {1, 5, 2, 9}, 0);
    test("falling", 2, {5, 3, 1}, 0);
}
