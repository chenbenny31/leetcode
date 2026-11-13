// 14 · Koko Eating Bananas (Binary Search on the Answer) — LC 875, 1011, 1891
// Mother problem: select/04-binary-search/04-koko-bananas.cpp
//
// Google reports: bisecting square area (58) ×3, minimax path, cutting ribbons
// (`maxRibbonLength`), min workers for a target
//
// LC 875: piles[i] bananas; each hour Koko eats up to k bananas from one pile (a smaller
// pile still takes the whole hour). Return the minimum k that finishes within h hours.
//
// Clarify
// - h >= number of piles? → yes; otherwise no speed works (check first, return -1)
// - pile and h up to 1e9 → hours at k = 1 reach 1e13, use long long
//
// Core idea
// - hours(k) never grows as k grows → lower bound for the first k with hours(k) <= h.
// - lo = 1 (k = 0 divides by zero), hi = max pile (always feasible).
// - ceil as (pile - 1) / k + 1: pile + k - 1 overflows int.
//
// Follow-ups
// - cutting ribbons (LC 1891) → `maxRibbonLength` below: last feasible, mid rounds up
// - ship within days (LC 1011) → same lower bound, lo = max weight, hi = sum, check =
//   greedy day count
// - maximize the minimum gap (LC 1552) → last feasible like ribbons on sorted positions,
//   check = greedily place balls at least gap apart
// - real-valued answer (bisecting squares, 58) → halve a fixed 100 times on doubles
// - minimax path → binary search on the limit + BFS check: `minimaxBinarySearch` in
//   select/09-advanced-graphs/04-swim-rising-water.cpp

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

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

// Lower bound on k in [1, max pile] for the first feasible (hours <= h).
// T: O(n log(max pile)), S: O(1).
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

// Pieces of this length cut from all ribbons; long long: many short pieces can exceed
// int. T: O(n), S: O(1).
long long piecesCut(const vector<int>& ribbons, int length) {
    long long pieces = 0;
    for (int ribbon : ribbons) {
        pieces += ribbon / length;
    }
    return pieces;
}

// LC 1891: largest integer length with at least k pieces, 0 if none. Last feasible on
// length in [0, max ribbon]; lo = 0 stands for "none". T: O(n log(max ribbon)), S: O(1).
int maxRibbonLength(const vector<int>& ribbons, int k) {
    int lo = 0;
    int hi = 0;
    for (int ribbon : ribbons) {
        hi = max(hi, ribbon);
    }
    while (lo < hi) {
        // round up so lo = mid always makes progress (and mid >= 1)
        int mid = lo + (hi - lo + 1) / 2;
        if (piecesCut(ribbons, mid) >= k) {
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

void testRibbon(const string& label, const vector<int>& ribbons, int k, int want) {
    cout << label << (maxRibbonLength(ribbons, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("int-max-piles", {INT_MAX, INT_MAX}, INT_MAX, 3);
    testRibbon("ribbon-across", {9, 7, 5}, 3, 5);
    testRibbon("ribbon-split-one", {10}, 3, 3);
    testRibbon("ribbon-none", {2, 1}, 4, 0);
}
