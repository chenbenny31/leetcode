// prices[i] = stock price on day i; hold at most one share. One function covers three
// rules: SINGLE (at most one buy and one sell), UNLIMITED (any number of trades), and
// FEE (unlimited trades, each sale pays fee). Return the max profit.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

enum Mode { SINGLE, UNLIMITED, FEE };

// Two states per day: cash = best profit holding nothing, hold = best profit holding a
// share. The rules differ only in what a buy starts from (0 for SINGLE, since the one
// trade can't reuse earlier profit) and whether a sale pays the fee. T: O(n), S: O(1).
long long maxProfit(const vector<int>& prices, Mode mode, int fee) {
    long long cash = 0, hold = LLONG_MIN / 2;
    for (int p : prices) {
        long long buyFrom = mode == SINGLE ? 0 : cash;
        long long sellFee = mode == FEE ? fee : 0;
        // hold uses yesterday's cash, so compute it before cash changes
        long long newHold = max(hold, buyFrom - p);
        cash = max(cash, hold + p - sellFee);
        hold = newHold;
    }
    return cash;
}

void test(const string& label, const vector<int>& prices, Mode mode, int fee,
          long long want) {
    cout << label << (maxProfit(prices, mode, fee) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {3, 1, 4, 2, 5}, SINGLE, 0, 4);
    test("unlimited", {3, 1, 4, 2, 5}, UNLIMITED, 0, 6);
    test("fee", {3, 1, 4, 2, 5}, FEE, 1, 4);
    test("fee-merge", {1, 3, 2, 8}, FEE, 2, 5);
    test("falling", {5, 4, 3}, UNLIMITED, 0, 0);
    test("fee-too-big", {1, 3}, FEE, 5, 0);
}
