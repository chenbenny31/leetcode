// span counts today itself, span >= 1
// equal price counts as <=
// array -> follow-up by stream input
// empty input -> empty result

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// mono stack of indices with strictly desc prices
// pop every price <= today's, idx on top is prev higher day, T: O(n), S: O(n)
vector<int> stockSpan(const vector<int>& prices) {
    int n = prices.size();
    vector<int> stk; // indices whose prices is higher than every day so far
    vector<int> span(n); // span[i] = num of consec days ending at i

    for (int i = 0; i < n; i++) {
        while (!stk.empty() && prices[stk.back()] <= prices[i]) {
            stk.pop_back();
        }
        
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
