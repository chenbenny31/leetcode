// 55 · Best Time to Buy and Sell Stock / Kadane — LC 121, 53, 1911
// Mother problem: select/13-dp/11-best-time-stock.cpp
//
// Google reports: Kadane, alternating sum, max ends score (18), grid paths with
// checkpoints (76)
//
// LC 121: prices[i] = price on day i; buy once, then sell on a later day. Return the max
// profit, 0 if no profit is possible.
//
// Clarify
// - buy once, sell on a later day; no profit → 0; at least one day
//
// Core idea
// - Keep the cheapest earlier price; answer = max(price − minPrice).
// - T O(n), S O(1).
//
// Tests
// - {3, 1, 4, 2} → 3; min comes late {2, 5, 1, 3} → 3; falling → 0; one day → 0
//
// Follow-ups
// - unlimited transactions (LC 122) → sum every positive day-to-day rise
// - max subarray (LC 53, Kadane) → ending = max(x, ending + x), best = max(best, ending);
//   subarray non-empty, so all negative returns the largest element
// - max alternating subsequence sum (LC 1911) → two states like buy / sell:
//   odd = max(odd, even + x), even = max(even, odd − x), return odd (long long)
// - at most k transactions (LC 188), cooldown (LC 309) → buy[j] / sell[j] states, O(n k)

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
