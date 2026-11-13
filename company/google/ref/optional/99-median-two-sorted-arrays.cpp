// 99 · Median of Two Sorted Arrays — LC 4
// Mother: select/optional/04-binary-search/06-median-two-sorted-arrays.cpp
// median of two sorted arrays as a double; even total -> mean of middle two
// reported: LC tag #3; 1p3a x2
//
// clarify:
// m + n >= 1 -> one array may be empty
// dups -> count separately
// required complexity -> O(log(m + n)); merge is O(m + n)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// k-th smallest (1-based): drop the rank / 2 block w/ smaller last element; O(log k)
int kthSmallest(const vector<int>& a, const vector<int>& b, int k) {
    int lenA = a.size();
    int lenB = b.size();
    // a[startA..] and b[startB..] still in play
    int startA = 0;
    int startB = 0;
    // rank of answer among elements still in play
    int rank = k;
    while (startA < lenA && startB < lenB && rank > 1) {
        int takeA = min(rank / 2, lenA - startA);
        int takeB = min(rank / 2, lenB - startB);
        int lastA = a[startA + takeA - 1];
        int lastB = b[startB + takeB - 1];
        if (lastA <= lastB) {
            startA += takeA;
            rank -= takeA;
        } else {
            startB += takeB;
            rank -= takeB;
        }
    }
    // one array used up -> answer sits in the other
    if (startA == lenA) {
        return b[startB + rank - 1];
    }
    if (startB == lenB) {
        return a[startA + rank - 1];
    }
    // rank == 1 -> smaller of the two heads
    return min(a[startA], b[startB]);
}

// core idea: median = avg of ((n+1)/2)-th and ((n+2)/2)-th smallest; same one if n odd
// T: O(log(m + n)), S: O(1)
double findMedianSortedArrays(const vector<int>& a, const vector<int>& b) {
    int n = a.size() + b.size();
    int lower = kthSmallest(a, b, (n + 1) / 2);
    int upper = kthSmallest(a, b, (n + 2) / 2);
    return (lower + upper) / 2.0;
}

void test(const string& label, const vector<int>& a, const vector<int>& b,
          double want) {
    cout << label << (findMedianSortedArrays(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-total", {1, 4, 7}, {2, 3}, 3.0);
    test("even-total", {1, 5}, {2, 8}, 3.5);
    test("one-empty", {}, {1, 2, 3, 4}, 2.5);
    test("short-runs-out", {1}, {2, 3, 4, 5, 6}, 3.5);
}

// follow-up:
// O(log min(m, n)) -> binary search cut i in shorter a, b's cut j fills left half
// cut valid -> a[i - 1] <= b[j] and b[j - 1] <= a[i]
// k-th of K sorted arrays -> min-heap of (value, array, idx), pop k times
// median of two run-length-encoded arrays -> walk both by runs to the middle cnt
