// LC 309 Best Time to Buy and Sell Stock with Cooldown: prices[i] is the price on day i.
// Any number of trades, one share at a time, and no buying the day after a sell.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::max;
using std::string;
using std::vector;

// State machine with three states per day, each updated from yesterday's states.
// T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    int hold = INT_MIN;   // best profit while holding a share
    int sold = 0;         // best profit if we sold today (tomorrow is the cooldown)
    int rest = 0;         // best profit with no share and free to buy today
    for (int p : prices) {
        int newHold = max(hold, rest - p);
        int newSold = hold + p;
        int newRest = max(rest, sold);
        hold = newHold;
        sold = newSold;
        rest = newRest;
    }
    return max(sold, rest);
}

void test(const string& label, const vector<int>& prices, int want) {
    if (maxProfit(prices) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("cooldown-bites", {2, 5, 0, 3}, 3);
    test("rise", {1, 2, 3}, 2);
    test("fall", {3, 2, 1}, 0);
    test("empty", {}, 0);
}
