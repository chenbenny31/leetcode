// LC 518 Coin Change II: coins are distinct positive ints, each usable any number of
// times. Count the combinations (order ignored) that sum to amount; answer fits in int.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Unbounded knapsack counting combinations: coins outer, amounts ascending inner, so
// each multiset is built in one fixed coin order. T: O(amount * n), S: O(amount).
int change(int amount, const vector<int>& coins) {
    vector<unsigned> ways(amount + 1, 0);   // ways[a] = combinations summing to a
    ways[0] = 1;
    for (int coin : coins) {
        for (int a = coin; a <= amount; a++) {
            ways[a] += ways[a - coin];
        }
    }
    return ways[amount];
}

void test(const string& label, int amount, const vector<int>& coins, int want) {
    cout << label << (change(amount, coins) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("order-ignored", 4, {1, 2}, 3);
    test("impossible", 3, {2}, 0);
    test("zero-amount", 0, {7}, 1);
}
