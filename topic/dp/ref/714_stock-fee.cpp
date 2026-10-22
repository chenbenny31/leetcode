// LC 714 Best Time to Buy and Sell Stock with Transaction Fee: prices[i] is the price on
// day i. Any number of trades, one share at a time, paying fee per completed trade.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::max;
using std::string;
using std::vector;

// State machine with two states per day. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices, int fee) {
    int cash = 0;         // best profit while holding no share
    int hold = INT_MIN;   // best profit while holding a share
    for (int p : prices) {
        // hold is updated first and is finite after day one, so hold + p - fee is safe
        hold = max(hold, cash - p);
        cash = max(cash, hold + p - fee);
    }
    return cash;
}

void test(const string& label, const vector<int>& prices, int fee, int want) {
    if (maxProfit(prices, fee) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("two-trades", {1, 4, 2, 6}, 1, 5);
    test("fee-too-big", {1, 2}, 5, 0);
    test("one-trade", {3, 9}, 2, 4);
    test("empty", {}, 1, 0);
}
