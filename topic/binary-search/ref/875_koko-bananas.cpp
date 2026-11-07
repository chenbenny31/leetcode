// LC 875 Koko Eating Bananas: min eating speed k (one pile per hour) to finish all piles
// within h hours.
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
    cout << label << (minEatingSpeed(piles, h) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("tight", {4, 9}, 3, 5);
    test("one-hour-per-pile", {4, 9}, 2, 9);
    test("plenty-of-time", {4, 9}, 100, 1);
    test("single-pile", {7}, 3, 3);
}
