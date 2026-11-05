// ByteDance phone (FT): ponds start with hourly yields yields[i]; there are `hours`
// hours. Each hour pick one pond, collect its current yield, then that pond's yield
// drops by 1 (never below 0). Return the maximum total catch. The interviewer wanted
// arithmetic over yield levels, faster than simulating hour by hour with a heap.
//   maximizeFish(yields, hours) -> long long
// Constraints: n <= 2e5, yields[i] <= 1e9, hours <= 1e14, answer fits in 64 bits.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::sort;
using std::greater;

// Straightforward: greedy - every hour take the pond with the largest current yield.
// Only usable for small `hours`. T: O(hours log n), S: O(n).
long long maximizeFishHeap(const vector<int>& yields, long long hours) {
    // current yields, largest on top
    priority_queue<long long> heap(yields.begin(), yields.end());
    long long total = 0;
    for (long long h = 0; h < hours; h++) {
        long long best = heap.top();
        heap.pop();
        if (best == 0) {
            break;
        }
        total += best;
        heap.push(best - 1);
    }
    return total;
}

// Sum of the integers lo..hi (empty when lo > hi).
long long rangeSum(long long lo, long long hi) {
    if (lo > hi) {
        return 0;
    }
    return (lo + hi) * (hi - lo + 1) / 2;
}

// Improved: sort descending; between the top-k level `cur` and the next yield `nxt`,
// each level gives k fish, so take whole bands of levels at once. T: O(n log n), S: O(n).
long long maximizeFish(vector<int> yields, long long hours) {
    sort(yields.begin(), yields.end(), greater<int>());
    int n = yields.size();
    long long total = 0;
    int k = 0;
    while (k < n && hours > 0) {
        long long cur = yields[k];
        // ponds tied at the current top level join the band together
        while (k < n && yields[k] == cur) {
            k++;
        }
        long long nxt = 0;
        if (k < n) {
            nxt = yields[k];
        }

        // the band (nxt, cur] has cur - nxt levels with k fish each
        long long bandHours = k * (cur - nxt);
        if (hours >= bandHours) {
            total += k * rangeSum(nxt + 1, cur);
            hours -= bandHours;
        } else {
            // hours run out inside the band: full levels, then `rest` ponds one lower
            long long fullLevels = hours / k;
            long long rest = hours % k;
            total += k * rangeSum(cur - fullLevels + 1, cur);
            total += rest * (cur - fullLevels);
            hours = 0;
        }
    }
    return total;
}

void test(const string& label, const vector<int>& yields, long long hours,
          long long want) {
    bool ok = maximizeFishHeap(yields, hours) == want;
    ok = ok && maximizeFish(yields, hours) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-pond", {5}, 3, 5 + 4 + 3);
    test("ties", {3, 3}, 3, 3 + 3 + 2);
    test("mid-band", {4, 1}, 4, 4 + 3 + 2 + 1);
    test("run-out-of-fish", {1, 0}, 10, 1);
    test("split-level", {3, 2, 2}, 5, 3 + 2 + 2 + 2 + 1);
}

// follow-up: beat the heap -> see maximizeFish() above; sorted bands of levels, take
//   k * rangeSum per band, O(n log n)
// follow-up: no sort / answer mod 1e9+7 (LC 1648) -> binary search smallest T with
//   cnt(T) = sum max(0, y - T) <= hours; take all levels > T, plus (hours - cnt) * T
// follow-up: per-pond drop d[i] -> same threshold search, cnt_i(T) = ceil((y - T) / d)
//   for y > T, O(n log maxY)
