// LC 4 Median of Two Sorted Arrays: return the median of two sorted arrays with
// m + n >= 1 elements in O(log(m + n)) time.
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <cmath>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::fabs;
using std::max;
using std::min;

// Binary search the cut i in the shorter array; the cut j = half - i in the longer one
// makes the left parts hold half of all elements. The cut is right when each left max
// is <= the other side's right min. T: O(log(min(m, n))), S: O(1).
double findMedianSortedArrays(const vector<int>& x, const vector<int>& y) {
    const vector<int>& a = x.size() <= y.size() ? x : y;   // shorter
    const vector<int>& b = x.size() <= y.size() ? y : x;
    int m = a.size();
    int n = b.size();
    int half = (m + n + 1) / 2;   // size of the combined left part
    int lo = 0;
    int hi = m;
    while (lo <= hi) {
        int i = lo + (hi - lo) / 2;   // a[0..i) is in the left part
        int j = half - i;             // b[0..j) is in the left part
        int aLeft = i > 0 ? a[i - 1] : INT_MIN;
        int aRight = i < m ? a[i] : INT_MAX;
        int bLeft = j > 0 ? b[j - 1] : INT_MIN;
        int bRight = j < n ? b[j] : INT_MAX;
        if (aLeft > bRight) {
            hi = i - 1;
        } else if (bLeft > aRight) {
            lo = i + 1;
        } else if ((m + n) % 2 == 1) {
            return max(aLeft, bLeft);
        } else {
            return (0.0 + max(aLeft, bLeft) + min(aRight, bRight)) / 2;
        }
    }
    return 0;
}

void test(const string& label, const vector<int>& a, const vector<int>& b, double want) {
    if (fabs(findMedianSortedArrays(a, b) - want) < 1e-9) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("odd-total", {1, 3}, {2}, 2);
    test("even-total", {1, 2}, {3, 4}, 2.5);
    test("one-empty", {}, {5}, 5);
    test("all-equal", {1, 1}, {1, 1}, 1);
}
