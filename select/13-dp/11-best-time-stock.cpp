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

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {3, 1, 4, 2}, 3);
    test("late-min", {2, 5, 1, 3}, 3);
    test("falling", {3, 2, 1}, 0);
    test("one-day", {5}, 0);
}

// follow-up: unlimited transactions (LC 122) -> add every positive prices[i] -
//   prices[i - 1]
// follow-up: at most k transactions (LC 188) -> dp states buy[j] / sell[j] per j <= k,
//   O(n * k); cooldown variant is LC 309
