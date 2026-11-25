// 62 · Best Time to Buy and Sell Stock / Kadane — LC 121, 53, 1911
// Mother: select/13-dp/11-best-time-stock.cpp
// prices[i] = price on day i; buy once, sell on a later day; return max profit
// reported: Kadane, alternating sum, max ends score (19), grid checkpoints (82)
//
// clarify:
// sell strictly after buy -> same day = profit 0
// no profit possible -> 0
// at least one day -> prices[0] is safe

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// core idea: keep the cheapest earlier price; answer = max(price - minPrice)
// T: O(n), S: O(1)
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

// follow-up:
// unlimited transactions (LC 122) -> sum every positive day-to-day rise
// max subarray (LC 53, Kadane) -> ending = max(x, ending + x), best = max(best, ending)
// alternating subseq sum (LC 1911) -> odd = max(odd, even + x), even = max(even, odd - x)
// at most k transactions (LC 188), cooldown (LC 309) -> buy[j] / sell[j] states, O(n k)
