// LC 123 Best Time to Buy and Sell Stock III: max profit with at most two transactions,
// holding at most one share at a time.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::max;
using std::string;
using std::vector;

// Four-state machine: buy1 -> sell1 -> buy2 -> sell2. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    // buyK / sellK = best profit just after the K-th buy / sell
    int buy1 = -prices[0], sell1 = 0, buy2 = -prices[0], sell2 = 0;

    for (int p : prices) {
        buy1 = max(buy1, -p);
        sell1 = max(sell1, buy1 + p);
        buy2 = max(buy2, sell1 - p);
        sell2 = max(sell2, buy2 + p);
    }

    return sell2;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-trades", {3, 1, 4, 2, 6}, 7);
    test("one-trade", {1, 2, 3, 4}, 3);
    test("falling", {4, 3, 1}, 0);
    test("single-day", {5}, 0);
}
