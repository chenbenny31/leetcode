// LC 121: prices[i] = stock price on day i; one buy then one later sell. Return the max
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

// Track the cheapest buy so far; sell today against it. T: O(n), S: O(1).
int maxProfit(const vector<int>& prices) {
    int minPrice = prices[0];   // cheapest price on any earlier day
    int best = 0;
    for (int p : prices) {
        best = max(best, p - minPrice);
        minPrice = min(minPrice, p);
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
    test("one", {5}, 0);
}
