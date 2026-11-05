// ByteDance (hack2hire) / LC 2226: candies[i] is a pile; piles can be split but not
// merged. Give k children the same number of candies each, every child's share from a
// single pile. Return the max candies per child (0 if impossible).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// number of children that can get v candies each
long long children(const vector<int>& candies, int v) {
    long long total = 0;
    for (int c : candies) {
        total += c / v;
    }
    return total;
}

// Binary search the largest v with children(v) >= k.
// T: O(n log M), S: O(1); M = max pile.
int maximumCandies(const vector<int>& candies, long long k) {
    // answer in [lo, hi]; lo = 0 is always feasible
    int lo = 0;
    int hi = 0;
    for (int c : candies) {
        hi = max(hi, c);
    }
    while (lo < hi) {
        // round up so lo = mid always makes progress
        int mid = lo + (hi - lo + 1) / 2;
        if (children(candies, mid) >= k) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& candies, long long k, int want) {
    cout << label << (maximumCandies(candies, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {5, 8, 6}, 3, 5);
    test("split-one-pile", {10}, 3, 3);
    test("impossible", {2, 5}, 11, 0);
}

// follow-up: Koko bananas (LC 875) -> smallest v with sum(ceil(p / v)) <= h; same
//   pattern, lower-bound version
// follow-up: split array largest sum (LC 410) -> binary search the max sum; greedy
//   count of pieces <= m
// follow-up: piles may be merged -> answer = sum(candies) / k, O(n)
