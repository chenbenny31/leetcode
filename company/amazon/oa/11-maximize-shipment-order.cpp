// shipmentOrder is a permutation of 1..n. Choose exactly one contiguous window of length
// windowSize and sort it ascending; everything else stays. Return the lexicographically
// largest array you can get.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::is_sorted;

// Sorting a window never makes it larger, so an already-sorted window gives back the
// original array (the best possible). Otherwise try every window and keep the largest.
// T: O(n * k log k) for k = windowSize, S: O(n).
vector<int> maximizeOrder(const vector<int>& order, int k) {
    int n = order.size();
    for (int i = 0; i + k <= n; i++) {
        if (is_sorted(order.begin() + i, order.begin() + i + k)) { return order; }
    }
    vector<int> best;
    for (int i = 0; i + k <= n; i++) {
        vector<int> cur = order;
        sort(cur.begin() + i, cur.begin() + i + k);
        if (best.empty() || cur > best) { best = cur; }
    }
    return best;
}

void test(const string& label, const vector<int>& order, int k, const vector<int>& want) {
    cout << label << (maximizeOrder(order, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted-window", {2, 1, 4, 3}, 2, {2, 1, 4, 3});
    test("late-window", {3, 2, 1}, 2, {3, 1, 2});
    test("whole", {2, 1, 3}, 3, {1, 2, 3});
    test("k-one", {2, 1}, 1, {2, 1});
}
