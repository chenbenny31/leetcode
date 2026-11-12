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

// LC 122: add every positive day-to-day rise. T: O(n), S: O(1).
int maxProfitUnlimited(const vector<int>& prices) {
    int total = 0;
    int n = prices.size();
    for (int i = 1; i < n; i++) {
        // buy yesterday, sell today only when it gains; this sums every upswing
        total += max(0, prices[i] - prices[i - 1]);
    }
    return total;
}

// LC 53 (Kadane): best sum ending here = max(x, ending + x). T: O(n), S: O(1).
int maxSubArray(const vector<int>& nums) {
    // best sum of a subarray that ends at the current element
    int ending = nums[0];
    int best = nums[0];
    int n = nums.size();
    for (int i = 1; i < n; i++) {
        // extend the previous run, or start fresh when it only hurts
        ending = max(nums[i], ending + nums[i]);
        best = max(best, ending);
    }
    return best;
}

// LC 1911: subsequence sum a0 - a1 + a2 - ...; two states like buy / sell: adding x is
// a sell, subtracting x is a buy. T: O(n), S: O(1).
long long maxAlternatingSum(const vector<int>& nums) {
    // best sum of an even-length pick so far (empty = 0); the next pick is added
    long long even = 0;
    // best sum of an odd-length pick so far; the next pick is subtracted
    // (values are positive, so 0 is a safe start)
    long long odd = 0;
    for (int num : nums) {
        long long nextOdd = max(odd, even + num);
        long long nextEven = max(even, odd - num);
        odd = nextOdd;
        even = nextEven;
    }
    // ending on a subtracted value never helps
    return odd;
}

void test(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfit(prices) == want ? " PASS\n" : " FAIL\n");
}

void testUnlimited(const string& label, const vector<int>& prices, int want) {
    cout << label << (maxProfitUnlimited(prices) == want ? " PASS\n" : " FAIL\n");
}

void testKadane(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxSubArray(nums) == want ? " PASS\n" : " FAIL\n");
}

void testAlternating(const string& label, const vector<int>& nums, long long want) {
    cout << label << (maxAlternatingSum(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {3, 1, 4, 2}, 3);
    test("late-min", {2, 5, 1, 3}, 3);
    test("falling", {3, 2, 1}, 0);
    test("one-day", {5}, 0);
    testUnlimited("two-rises", {1, 3, 2, 5}, 5);
    testUnlimited("steady-climb", {1, 2, 3, 4}, 3);
    testUnlimited("all-falling", {4, 3, 1}, 0);
    testKadane("kadane-mixed", {2, -3, 4, -1, 2}, 5);
    testKadane("kadane-all-negative", {-3, -1, -2}, -1);
    testKadane("kadane-all-positive", {1, 2, 3}, 6);
    testAlternating("alt-zigzag", {5, 1, 5}, 9);
    testAlternating("alt-rising", {1, 2, 3}, 3);
    testAlternating("alt-one", {4}, 4);
}

// follow-up: at most k transactions (LC 188) -> dp states buy[j] / sell[j] per j <= k,
//   O(n * k); cooldown variant is LC 309
