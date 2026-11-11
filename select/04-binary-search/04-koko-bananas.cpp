// LC 875 Koko Eating Bananas (Medium). Pattern: binary search on the answer.
// piles[i] bananas; each hour Koko eats up to k bananas from one pile (a smaller pile
// still takes the whole hour). Return the minimum k that finishes within h hours.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

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

void test(const string& label, const vector<int>& piles, int h, int want) {
    cout << label << (minEatingSpeed(piles, h) == want ? " PASS\n" : " FAIL\n");
}

void testShip(const string& label, const vector<int>& weights, int days, int want) {
    cout << label << (shipWithinDays(weights, days) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("single-pile", {7}, 3, 3);
    testShip("ship-two-days", {3, 2, 2, 4, 1}, 2, 7);
    testShip("ship-one-day", {3, 2, 2, 4, 1}, 1, 12);
    testShip("ship-max-weight", {3, 2, 2, 4, 1}, 5, 4);
}

// follow-up: recognize it -> "minimize X such that check(X) passes" + check is
//   monotonic in X; same shape: split array largest sum (LC 410)
