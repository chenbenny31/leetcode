// return the max profit, only one buy then one later sell, 0 if no profit

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// track the cheapest buy so far, comp update cur profit, T: O(n), S: O(1)
int maxProfit(const vector<int>& prices) {
    int minPrice = prices[0];
    int maxProfit = 0;

    for (int p : prices) {
        maxProfit = max(maxProfit, p - minPrice);
        minPrice = min(minPrice, p);
    }
    return maxProfit;
}

// state machine, T: O(n), S: O(1)
int maxProfitStates(const vector<int>& prices) {
    // two parallel state carrying
    int hold = -prices[0]; // state when holding stock -> cur money
    int sold = 0; // state after selling stock -> cur money

    for (int p : prices) {
        sold = max(sold, hold + p);  // sold could update by (hold + sell today)
        hold = max(hold, -p); // hold could update by (0 + buy tody)
    }
    return sold;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {3, 1, 4, 2}, 3);
    test("late-min", {2, 5, 1, 3}, 3);
    test("falling", {3, 2, 1}, 0);
    test("one", {5}, 0);
}
