// product is a 0/1 array. One operation picks a window of length k, costs the current
// sum of that window, and turns one 1 inside it into 0. Return the min total cost to turn
// every 1 into 0. (Amazon OA "Minimum Cost to Convert Products to Variant A".)

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Let m = number of 1s and c = fewest 1s in any length-k window. Lower bound: after j
// operations every window still holds at least c - j ones, so operation j costs at least
// max(1, c - j). It is reached: empty the sparsest window first (c, c-1, ..., 1), then
// remove the other 1s from nearest to farthest, each with a window ending (or starting)
// at it whose other cells are already 0, cost 1 each. Answer = m + c(c-1)/2.
// straightforward: count every window directly. T: O(n * k), S: O(1).
long long minCost(const vector<int>& product, int k) {
    int n = product.size();
    long long m = 0;
    for (int x : product) { m += x; }
    long long c = LLONG_MAX;
    for (int s = 0; s + k <= n; s++) {
        long long ones = 0;
        for (int i = s; i < s + k; i++) { ones += product[i]; }
        c = min(c, ones);
    }
    return m + c * (c - 1) / 2;
}

// improved: slide the window, adding the new cell and dropping the old one.
// T: O(n), S: O(1).
long long minCostSliding(const vector<int>& product, int k) {
    int n = product.size();
    long long m = 0, ones = 0;
    for (int x : product) { m += x; }
    for (int i = 0; i < k; i++) { ones += product[i]; }
    long long c = ones;
    for (int i = k; i < n; i++) {
        ones += product[i] - product[i - k];
        c = min(c, ones);
    }
    return m + c * (c - 1) / 2;
}

void test(const string& label, const vector<int>& product, int k, long long want) {
    bool ok = minCost(product, k) == want && minCostSliding(product, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("isolated", {1, 0, 0, 1}, 2, 2);
    test("all-ones", {1, 1, 1, 1}, 3, 7);
    test("sparse-window", {1, 1, 0, 1, 1}, 4, 7);
    test("whole", {1, 0, 1}, 3, 3);
    test("none", {0, 0}, 1, 0);
}
