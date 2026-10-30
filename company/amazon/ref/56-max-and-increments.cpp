// LC 3806 (statement paraphrased; check exact limits): you may add 1 to elements of nums
// at most k times in total. Choose exactly m elements; return the max possible bitwise
// AND of the m chosen values after the increments.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// smallest y >= x whose bits include all bits of target, minus x. Either x already
// works, or y keeps x above some bit p where x has 0, sets p, and below p uses exactly
// target's bits; take the cheapest valid p.
long long cost(long long x, long long target) {
    if ((x & target) == target) { return 0; }
    long long best = -1;
    for (int p = 0; p < 62; p++) {
        if (x >> p & 1) { continue; }
        long long high = x >> (p + 1) << (p + 1);
        long long highTarget = target >> (p + 1) << (p + 1);
        if ((high & highTarget) != highTarget) { continue; }
        long long y = high | (1LL << p) | (target & ((1LL << p) - 1));
        if (best == -1 || y - x < best) { best = y - x; }
    }
    return best;
}

// Greedy from the top bit: keep bit b if the m cheapest elements can all be raised to
// contain (answer | b) within k. Feasibility only gets easier for subsets of bits, so
// this finds the max. T: O(B * (n * B + n log n)) for B bits, S: O(n).
long long maximumAnd(const vector<int>& nums, long long k, int m) {
    long long answer = 0;
    for (int b = 40; b >= 0; b--) {
        long long target = answer | (1LL << b);
        vector<long long> costs;
        for (int x : nums) { costs.push_back(cost(x, target)); }
        sort(costs.begin(), costs.end());
        long long total = 0;
        for (int i = 0; i < m && total <= k; i++) { total += costs[i]; }
        if (total <= k) { answer = target; }
    }
    return answer;
}

void test(const string& label, const vector<int>& nums, long long k, int m,
          long long want) {
    cout << label << (maximumAnd(nums, k, m) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pick-max", {1, 2, 4}, 0, 1, 4);
    test("no-ops", {1, 2}, 0, 2, 0);
    test("raise-one", {3, 4}, 1, 2, 4);
    test("fill-bits", {6}, 1, 1, 7);
    test("share", {0, 0, 0}, 6, 3, 2);
}
