// 14 · Koko Eating Bananas (Binary Search on the Answer) — LC 875, 1011, 1891
// Mother: select/04-binary-search/04-koko-bananas.cpp
// (1) LC 875: each hour eat up to k bananas from one pile; min k to finish within h hours
// (2) LC 1891: cut ribbons into >= k pieces of one integer len; max such len, 0 if none
// reported: bisecting squares (58) x3, minimax path, cutting ribbons, min workers
//
// clarify:
// h vs num of piles -> h >= piles, so an answer exists
// size of piles and h -> total hours can pass int, use long long
// (2) leftover ribbon -> thrown away
// (2) no valid len -> 0

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::max;

// hours needed at speed k; long long: n piles at k = 1 can exceed int
long long hoursNeeded(const vector<int>& piles, int k) {
    long long hours = 0;
    for (int pile : piles) {
        // ceil(pile / k) for pile >= 1; pile + k - 1 could overflow int
        hours += (pile - 1) / k + 1;
    }
    return hours;
}

// (1) min eating speed (LC 875)
// core idea: hours(k) never grows w/ k -> first k in [1, max pile] w/ hours(k) <= h
// T: O(n log(max pile)), S: O(1)
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

// pieces of this len cut from all ribbons; long long: cnt can exceed int
long long piecesCut(const vector<int>& ribbons, int length) {
    long long pieces = 0;
    for (int ribbon : ribbons) {
        pieces += ribbon / length;
    }
    return pieces;
}

// (2) max ribbon len (LC 1891)
// core idea: pieces(len) never grows w/ len -> last feasible len in [0, max ribbon]
// lo = 0 stands for "none"
// T: O(n log(max ribbon)), S: O(1)
int maxRibbonLength(const vector<int>& ribbons, int k) {
    int lo = 0;
    int hi = 0;
    for (int ribbon : ribbons) {
        hi = max(hi, ribbon);
    }
    while (lo < hi) {
        // round up -> lo = mid always makes progress (and mid >= 1)
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

// follow-up:
// ship within days (LC 1011) -> same lower bound; lo = max weight, hi = sum, greedy days
// max the min gap (LC 1552) -> last feasible like (2) on sorted pos; greedy placement
// min workers for a target -> first feasible like (1): lower bound on the worker cnt
// real-valued answer (bisecting squares, 58) -> halve a fixed 100 times on doubles
// minimax path -> binary search on the limit + BFS check
// minimax path ref -> select/09-advanced-graphs/04-swim-rising-water.cpp
