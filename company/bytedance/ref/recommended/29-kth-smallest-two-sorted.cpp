// ByteDance (jobright, onsite): return the kth smallest element of two sorted arrays
// without merging them (1 <= k <= total length).
// Follow-up (jobright, MLE phone) LC 4: median of two sorted arrays in O(log(m + n)).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// straightforward: walk both arrays like a merge and stop at the kth. T: O(k), S: O(1).
int kthSmallestWalk(const vector<int>& a, const vector<int>& b, int k) {
    int lenA = a.size();
    int lenB = b.size();
    // next unread position in each array
    int posA = 0;
    int posB = 0;
    int value = 0;
    for (int taken = 0; taken < k; taken++) {
        // take the smaller head; an exhausted array never wins
        if (posB == lenB || (posA < lenA && a[posA] <= b[posB])) {
            value = a[posA];
            posA++;
        } else {
            value = b[posB];
            posB++;
        }
    }
    return value;
}

// improved: look at the next k/2 elements of each array. The block whose last element
// is smaller lies entirely below the kth, so drop it and look for a smaller rank.
// T: O(log k), S: O(1).
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
    if (startA == lenA) { return b[startB + rank - 1]; }
    if (startB == lenB) { return a[startA + rank - 1]; }
    // rank == 1: the smaller of the two heads
    return min(a[startA], b[startB]);
}

// follow-up LC 4: the median is the average of the ((n+1)/2)-th and ((n+2)/2)-th
// smallest (the same element when n is odd). T: O(log(m + n)), S: O(1).
double findMedianSortedArrays(const vector<int>& a, const vector<int>& b) {
    int n = a.size() + b.size();
    return (kthSmallest(a, b, (n + 1) / 2) + kthSmallest(a, b, (n + 2) / 2)) / 2.0;
}

void test(const string& label, const vector<int>& a, const vector<int>& b, int k,
          int want) {
    bool ok = kthSmallestWalk(a, b, k) == want && kthSmallest(a, b, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testMedian(const string& label, const vector<int>& a, const vector<int>& b,
                double want) {
    cout << label << (findMedianSortedArrays(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 4, 7}, {2, 3}, 3, 3);
    test("short-side-runs-out", {1}, {2, 3, 4, 5}, 4, 4);
    test("duplicates", {1, 1}, {1, 2}, 4, 2);
    testMedian("median-odd", {1, 3}, {2}, 2.0);
    testMedian("median-even-one-empty", {}, {1, 2, 3, 4}, 2.5);
}

// follow-up: median (LC 4) -> see findMedianSortedArrays() above; avg of two kth calls
// follow-up: median in O(log min(m, n)) -> binary search cut i in the shorter array,
//   j = (m + n + 1) / 2 - i; valid when a[i-1] <= b[j] && b[j-1] <= a[i]
// follow-up: kth of K sorted arrays -> min-heap of (val, arr, idx), pop k times,
//   O(k log K)
