// LC 875 Koko Eating Bananas (Medium) — binary search on the answer
// each hour eat up to k bananas from one pile; return min int k to finish within h hours
//
// clarify:
// h < num of piles -> no speed works; assume h >= piles (follow-up)
// small pile -> still takes the whole hour
// sizes -> total hours can exceed int, use long long
// k -> integer >= 1

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// hours needed at speed k; O(n)
long long hoursNeeded(const vector<int>& piles, int k) {
    // long long: many piles at k = 1 can exceed int
    long long hours = 0;
    for (int pile : piles) {
        // ceil(pile / k) for pile >= 1, w/o pile + k - 1 overflowing int
        hours += (pile - 1) / k + 1;
    }
    return hours;
}

// core idea: hours never grow as k grows -> find first feasible k in [lo, hi)
// speeds below lo are too slow; speeds from hi (max pile) on are feasible
// T: O(n log(max pile)), S: O(1)
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
            // mid is a candidate; answer is mid or slower
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

// follow-up:
// how to recognize -> "min X such that check(X) passes" and check is monotonic in X
// ship in D days (LC 1011), split array (LC 410) -> capacity in [max, sum], greedy check
// max the min gap (LC 1552) -> LAST feasible: lo = mid (mid rounded up) / hi = mid - 1
// cutting ribbons (LC 1891) -> last feasible len in [0, max]; lo = 0 means "none"
// h < num of piles -> return -1 before the search
// real-valued answer -> no + 1 / - 1 on doubles; halve ~100 times w/ lo = mid / hi = mid
// minimax path -> binary search on the limit w/ BFS check; 09-advanced-graphs/04
