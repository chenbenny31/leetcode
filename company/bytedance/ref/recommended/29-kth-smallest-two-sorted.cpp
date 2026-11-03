// ByteDance (jobright, onsite) : return the kth smallest element of two sorted arrays
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
    int na = a.size();
    int nb = b.size();
    int i = 0;
    int j = 0;
    int cur = 0;
    for (int step = 0; step < k; step++) {
        // take the smaller head; an exhausted array never wins
        if (j == nb || (i < na && a[i] <= b[j])) {
            cur = a[i];
            i++;
        } else {
            cur = b[j];
            j++;
        }
    }
    return cur;
}

// improved: compare the (k/2)-th remaining element of each array; the side with the
// smaller one cannot hold the kth in that block, so drop it. T: O(log k), S: O(1).
int kthSmallest(const vector<int>& a, const vector<int>& b, int k) {
    int na = a.size();
    int nb = b.size();
    // a[i..] and b[j..] are still in play; we want the kth of them
    int i = 0;
    int j = 0;
    while (true) {
        if (i == na) { return b[j + k - 1]; }
        if (j == nb) { return a[i + k - 1]; }
        if (k == 1) { return min(a[i], b[j]); }
        int half = k / 2;
        // last index of each block (clamped to the array end)
        int ia = min(i + half, na) - 1;
        int jb = min(j + half, nb) - 1;
        if (a[ia] <= b[jb]) {
            k -= ia - i + 1;
            i = ia + 1;
        } else {
            k -= jb - j + 1;
            j = jb + 1;
        }
    }
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
