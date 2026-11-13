// LC 875 Koko Eating Bananas (Medium) — binary search on the answer
// piles[i] bananas; each hour Koko eats up to k bananas from one pile (a smaller pile
// still takes the whole hour). Return the minimum integer k that finishes within h hours.
// Clarify: h >= number of piles (else no speed works); pile and h sizes (hours can
//   exceed int); k is an integer >= 1; return the speed, not the hours.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Hours needed at speed k. T: O(n), S: O(1).
long long hoursNeeded(const vector<int>& piles, int k) {
    // long long: many piles at k = 1 can exceed int
    long long hours = 0;
    for (int pile : piles) {
        // ceil(pile / k) for pile >= 1, without pile + k - 1 overflowing int
        hours += (pile - 1) / k + 1;
    }
    return hours;
}

// Core idea: hours needed never grows as k grows, so find the first feasible k in the
// half-open range [lo, hi): speeds below lo are too slow, speeds from hi on (max pile)
// are feasible. T: O(n log(max pile)), S: O(1).
int minEatingSpeed(const vector<int>& piles, int h) {
    int lo = 1;
    int hi = 1;
    for (int pile : piles) {
        hi = max(hi, pile);
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (hoursNeeded(piles, mid) > h) {
            // mid and every slower speed are too slow
            lo = mid + 1;
        } else {
            // mid is a candidate; the answer is mid or slower
            hi = mid;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& piles, int h, int want) {
    cout << label << (minEatingSpeed(piles, h) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("single-pile", {7}, 3, 3);
    return 0;
}

// follow-up (ideas only):
// - how to recognize it -> "minimize X such that check(X) passes" and check is monotonic
//   in X
// - ship packages within D days (LC 1011), split array largest sum (LC 410) -> same loop
//   on capacity in [max weight, sum]; check = greedy count of days / parts
// - maximize the minimum gap (LC 1552, aggressive cows) -> sort, find the LAST feasible
//   gap: feasible -> lo = mid with mid rounded up, else hi = mid - 1; greedy check
// - cutting ribbons (LC 1891) -> last feasible length in [0, max ribbon]; lo = 0 stands
//   for "none", and mid rounded up is never 0
// - h < number of piles -> no speed works; return -1 before the search
// - real-valued answer -> no + 1 / - 1 on doubles; halve a fixed number of times (~100)
//   with lo = mid / hi = mid
// - minimax path (smallest possible max cell on a path) -> binary search on the limit
//   with a BFS check; see 09-advanced-graphs/04-swim-rising-water
