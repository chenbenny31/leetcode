// 94 · Median of Two Sorted Arrays — LC 4
// All variants with code:
//   select/optional/04-binary-search/06-median-two-sorted-arrays.cpp
//
// Google reports: LC tag #3; 1p3a ×2
//
// Clarify
// - total size? → m + n ≥ 1; one array may be empty
// - duplicates? → allowed, they count as separate elements
// - return type? → double; even total → average of the two middle values
// - complexity asked? → O(log(m + n)) required (merging is O(m + n))
//
// Core idea
// - `kthSmallest`: compare the last element of the next rank/2 block in each array; the
//   block with the smaller last element lies below the answer, drop it and lower the
//   rank.
// - When one array is used up, index the other directly; at rank 1, take the smaller
//   head.
// - Median = average of the ((n+1)/2)-th and ((n+2)/2)-th smallest (same one for odd n).
// - T O(log(m + n)), S O(1).
//
// Tests: odd total, even total, one empty, short runs out
//
// Follow-ups
// - O(log min(m, n)) → binary search the cut i in the shorter array, j = (m+n+1)/2 − i;
//   valid when a[i−1] ≤ b[j] and b[j−1] ≤ a[i]
// - k-th of K sorted arrays → min-heap of (value, array, index), pop k times
// - median of two run-length-encoded arrays (onsite, Optional table) → walk both by runs
//   to the middle count
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// kth smallest (1-based) of a and b: compare the next rank/2 elements of each array.
// The block whose last element is smaller lies entirely below the answer, so drop it
// and look for a smaller rank. T: O(log k), S: O(1).
int kthSmallest(const vector<int>& a, const vector<int>& b, int k) {
    int lenA = a.size();
    int lenB = b.size();
    // a[startA..] and b[startB..] are still in play
    int startA = 0;
    int startB = 0;
    // rank of the answer among the elements still in play
    int rank = k;
    while (startA < lenA && startB < lenB && rank > 1) {
        // block sizes: rank / 2 each, fewer if the array runs out
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
    // one array is used up: the answer sits directly in the other
    if (startA == lenA) {
        return b[startB + rank - 1];
    }
    if (startB == lenB) {
        return a[startA + rank - 1];
    }
    // rank == 1: the smaller of the two heads
    return min(a[startA], b[startB]);
}

// Median = average of the ((n+1)/2)-th and ((n+2)/2)-th smallest (same one when n is
// odd). T: O(log(m + n)), S: O(1).
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
