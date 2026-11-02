// Stock span (LC 901 as an array): span[i] = number of consecutive days ending at day i
// whose price is <= prices[i].

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Monotonic stack of indices with strictly decreasing prices: pop every price <= today;
// the index left on top is the previous higher day. T: O(n), S: O(n).
vector<int> stockSpan(const vector<int>& prices) {
    int n = prices.size();
    // indices of days whose price is higher than every later day seen so far
    vector<int> stk;
    vector<int> span(n);
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && prices[stk.back()] <= prices[i]) { stk.pop_back(); }
        if (stk.empty()) {
            span[i] = i + 1;
        } else {
            span[i] = i - stk.back();
        }
        stk.push_back(i);
    }
    return span;
}

void test(const string& label, const vector<int>& prices, const vector<int>& want) {
    cout << label << (stockSpan(prices) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rise", {3, 1, 2, 5}, {1, 1, 2, 4});
    test("falling", {5, 4, 3}, {1, 1, 1});
    test("equal-counts", {2, 2}, {1, 2});
}
