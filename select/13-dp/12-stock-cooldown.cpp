// LC 309 Best Time to Buy and Sell Stock with Cooldown (Medium). Pattern: state DP.
// prices[i] is the price on day i. Any number of trades, holding at most one share,
// and no buying on the day right after a sell. Return the max profit.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Three states per day, each computed from yesterday's three. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    // best profit while holding a share
    int hold = INT_MIN;
    // best profit having sold today (tomorrow is the cooldown)
    int sold = 0;
    // best profit with no share and free to buy today
    int rest = 0;
    for (int price : prices) {
        int newHold = max(hold, rest - price);
        int newSold = hold + price;
        int newRest = max(rest, sold);
        hold = newHold;
        sold = newSold;
        rest = newRest;
    }
    return max(sold, rest);
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cooldown-bites", {2, 5, 0, 3}, 3);
    test("rise", {1, 2, 3}, 2);
    test("fall", {3, 2, 1}, 0);
    test("empty", {}, 0);
}

// follow-up: transaction fee (LC 714) -> two states: hold = max(hold, free - price),
//   free = max(free, hold + price - fee)
// follow-up: at most k transactions (LC 188) -> hold[j] / free[j] per j <= k, O(n * k)
