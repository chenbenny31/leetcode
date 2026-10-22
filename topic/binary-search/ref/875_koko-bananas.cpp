// LC 875 Koko Eating Bananas: piles[i] bananas; each hour Koko eats up to k bananas from
// one pile (a pile with fewer takes the whole hour). Return the minimum integer k so all
// piles are eaten within h hours (h >= number of piles).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Hours needed at eating speed k. T: O(n), S: O(1).
long long hoursNeeded(const vector<int>& piles, int k) {
    long long hours = 0;
    for (int p : piles) { hours += (p + k - 1) / k; }
    return hours;
}

// Binary search on the answer in [1, max pile]; feasible = hours <= h, which is monotonic
// in k. T: O(n * log(max pile)), S: O(1).
int minEatingSpeed(const vector<int>& piles, int h) {
    int lo = 1;
    int hi = 1;
    for (int p : piles) { hi = max(hi, p); }
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

void test(const string& label, const vector<int>& piles, int h, int want) {
    if (minEatingSpeed(piles, h) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("single-pile", {7}, 3, 3);
}
