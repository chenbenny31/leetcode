// One operation picks two different indices, adds 1 to one and subtracts 1 from the
// other (values may go negative). After k operations, return the max possible median,
// the element at index n / 2 of the sorted array. With n >= 2, "exactly k" and "at most
// k" give the same answer, because a spare operation can add 1 to an element above the
// median and take 1 from one below it. (hack2hire's first example contradicts its own
// "values may go negative" rule; this follows the rule.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// cost to lift the upper half (sorted indices m..n-1) to at least v
long long need(const vector<long long>& a, long long v) {
    int n = a.size();
    long long total = 0;
    for (int i = n / 2; i < n; i++) {
        if (a[i] < v) { total += v - a[i]; }
    }
    return total;
}

// Median >= v needs n - n/2 elements >= v; the cheapest are the largest ones, each unit
// of lifting costs one operation, and the matching -1 can always go to an element below
// the median. need(v) grows with v, so binary search it. T: O(n log n + n log k).
long long maxMedian(vector<long long> a, long long k) {
    sort(a.begin(), a.end());
    int n = a.size();
    if (n == 1) { return a[0]; }
    long long lo = a[n / 2], hi = a[n / 2] + k;
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        if (need(a, mid) <= k) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

void test(const string& label, const vector<long long>& a, long long k, long long want) {
    cout << label << (maxMedian(a, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lift-one", {1, 2, 3}, 1, 3);
    test("spare-op", {1, 2, 3}, 2, 3);
    test("lift-both", {1, 2, 3, 4}, 3, 5);
    test("pair", {4, 1}, 2, 6);
    test("no-ops", {5, 1, 3}, 0, 3);
}
