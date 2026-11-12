// LC 875 Koko Eating Bananas (Medium). Pattern: binary search on the answer.
// piles[i] bananas; each hour Koko eats up to k bananas from one pile (a smaller pile
// still takes the whole hour). Return the minimum k that finishes within h hours.
//
// Clarify:
// - h >= number of piles? -> yes (LC guarantees it); otherwise no speed works (-1)
// - up to 1e4 piles, a pile and h up to 1e9 -> hours at k = 1 reach 1e13: long long
// - k is an integer >= 1; one pile per hour, the rest of the hour is wasted
// - return the speed k, not the hours
// Idea: hours(k) never grows as k grows, so "hours(k) <= h" is false..false true..true
//   over k in [1, max pile]: lower bound for the first true. At k = max pile each pile
//   takes one hour, so hi is always feasible. O(n log max pile) instead of trying
//   every k.
// Pitfalls:
// - lo = 1, not 0: k = 0 divides by zero in hoursNeeded
// - hi = max pile (feasible, so the answer is inside [lo, hi]); no + 1 needed
// - feasible -> hi = mid (mid may be the answer), else lo = mid + 1; loop lo < hi
// - ceil as (pile - 1) / k + 1: (pile + k - 1) / k overflows int once pile + k >
//   INT_MAX; pile / k + 1 is wrong when k divides pile
// - sum the hours in long long (two INT_MAX piles at k = 1 already overflow int)
// - the loop returns lo even if no k is feasible: check h >= piles.size() first
// Tests to write: tight h; one hour per pile -> max pile; plenty of time -> 1; single
//   pile; INT_MAX piles (overflow in the ceil and in the sum).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::sort;

// Hours needed at speed k; long long: n piles at k = 1 can exceed int.
// T: O(n), S: O(1).
long long hoursNeeded(const vector<int>& piles, int k) {
    long long hours = 0;
    for (int pile : piles) {
        // ceil(pile / k) for pile >= 1, without pile + k - 1 overflowing int
        hours += (pile - 1) / k + 1;
    }
    return hours;
}

// Lower-bound template on k in [1, max pile]: feasible (hours <= h) is monotonic in k,
// so find the first feasible k. T: O(n log(max pile)), S: O(1).
int minEatingSpeed(const vector<int>& piles, int h) {
    int lo = 1;
    int hi = 1;
    for (int pile : piles) {
        hi = max(hi, pile);
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (hoursNeeded(piles, mid) <= h) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

// Days needed at this capacity: greedy, start a new day when the next package doesn't
// fit. Needs capacity >= max weight. T: O(n), S: O(1).
int daysNeeded(const vector<int>& weights, int capacity) {
    int days = 1;
    // weight already loaded on the current day
    int load = 0;
    for (int weight : weights) {
        if (load + weight > capacity) {
            days++;
            load = 0;
        }
        load += weight;
    }
    return days;
}

// LC 1011: lower-bound template on capacity in [max weight, sum]; feasible
// (days needed <= days) is monotonic, so find the first feasible.
// T: O(n log(sum)), S: O(1).
// Pitfall: lo = max weight, not 1; below it a package never fits and daysNeeded is
//   meaningless.
int shipWithinDays(const vector<int>& weights, int days) {
    // lo: must fit the heaviest package; hi: everything in one day
    int lo = 0;
    int hi = 0;
    for (int weight : weights) {
        lo = max(lo, weight);
        hi += weight;
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (daysNeeded(weights, mid) <= days) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

// Balls placed greedily left to right in sorted positions, each at least gap after the
// previous one. T: O(n), S: O(1).
int countPlaced(const vector<int>& positions, int gap) {
    int placed = 1;
    int lastPos = positions[0];
    for (int pos : positions) {
        if (pos - lastPos >= gap) {
            placed++;
            lastPos = pos;
        }
    }
    return placed;
}

// LC 1552 (aggressive cows): put m balls on distinct positions so the smallest gap
// between two balls is as large as possible; return that gap. Assumes 2 <= m <= n.
// "countPlaced >= m" is true..true false..false in the gap, so find the LAST true: the
// mirror of the lower bound. T: O(n log n + n log(range)), S: O(n).
// Pitfall: lo = mid needs mid = lo + (hi - lo + 1) / 2; rounding down loops forever
//   once hi == lo + 1 and mid is feasible.
int maxMinDistance(vector<int> positions, int m) {
    sort(positions.begin(), positions.end());
    // the largest feasible gap lies in [lo, hi]; gap 1 always fits m <= n balls
    int lo = 1;
    int hi = positions.back() - positions[0];
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;
        if (countPlaced(positions, mid) >= m) {
            // mid works; the answer is mid or larger
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& piles, int h, int want) {
    cout << label << (minEatingSpeed(piles, h) == want ? " PASS\n" : " FAIL\n");
}

void testShip(const string& label, const vector<int>& weights, int days, int want) {
    cout << label << (shipWithinDays(weights, days) == want ? " PASS\n" : " FAIL\n");
}

void testGap(const string& label, const vector<int>& positions, int m, int want) {
    cout << label << (maxMinDistance(positions, m) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("single-pile", {7}, 3, 3);
    test("int-max-piles", {INT_MAX, INT_MAX}, INT_MAX, 3);
    testShip("ship-two-days", {3, 2, 2, 4, 1}, 2, 7);
    testShip("ship-one-day", {3, 2, 2, 4, 1}, 1, 12);
    testShip("ship-max-weight", {3, 2, 2, 4, 1}, 5, 4);
    testGap("gap-pick-middle", {0, 3, 4, 10}, 3, 4);
    testGap("gap-unsorted", {7, 1, 5}, 2, 6);
    testGap("gap-every-spot", {1, 2, 3}, 3, 1);
    testGap("gap-adjacent", {0, 1, 3}, 2, 3);
}

// follow-up: recognize it -> "minimize X such that check(X) passes" + check is
//   monotonic in X; same shape: ship within days (LC 1011) -> shipWithinDays above;
//   split array largest sum (LC 410) -> select/optional/04-binary-search/05-split-array-
//   largest-sum.cpp
// follow-up: maximize the minimum (LC 1552, aggressive cows) -> maxMinDistance above
// follow-up: h < piles.size() -> no speed works; check before the search:
//     if ((long long)piles.size() > h) { return -1; }
// follow-up: real-valued answer (bisecting square area, G34) -> no + 1 / - 1 on
//   doubles; halve a fixed number of times instead of looping while lo < hi:
//     for (int iter = 0; iter < 100; iter++) {
//         double mid = (lo + hi) / 2;
//         if (feasible(mid)) { hi = mid; } else { lo = mid; }
//     }
// follow-up: minimax path (smallest possible max cell on a path) -> binary search on
//   the limit + BFS check: minimaxBinarySearch in
//   select/09-advanced-graphs/04-swim-rising-water.cpp
