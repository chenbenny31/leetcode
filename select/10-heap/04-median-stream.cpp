// LC 295 Find Median from Data Stream (Hard) — two heaps: lower half / upper half
// addNum(num) adds a value from a stream; findMedian() returns the median so far, the
// mean of the two middle values when the count is even.
// Clarify: findMedian before any addNum; value range (two middles can overflow int);
//   duplicates; number of calls (~5e4, so O(log n) add and O(1) median).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <cmath>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::fabs;

// Core idea: a max-heap low holds the smaller half and a min-heap high the larger half,
// low the same size as high or one more; the median sits on the tops.
// T: addNum O(log n), findMedian O(1), S: O(n).
class MedianFinder {
    // max-heap: smaller half, holds the extra value when the count is odd
    priority_queue<int> low;
    // min-heap: larger half
    priority_queue<int, vector<int>, greater<int>> high;

public:
    void addNum(int num) {
        // route through low so every value in low <= every value in high
        low.push(num);
        high.push(low.top());
        low.pop();
        if (high.size() > low.size()) {
            low.push(high.top());
            high.pop();
        }
    }

    double findMedian() {
        if (low.size() > high.size()) { return low.top(); }
        // double first: two large ints overflow when added as int
        return ((double)low.top() + high.top()) / 2.0;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MedianFinder mf;
    mf.addNum(5);
    check(fabs(mf.findMedian() - 5.0) < 1e-9, "one");
    mf.addNum(-2);
    check(fabs(mf.findMedian() - 1.5) < 1e-9, "even-negative");
    mf.addNum(9);
    check(fabs(mf.findMedian() - 5.0) < 1e-9, "odd");
    MedianFinder big;
    big.addNum(INT_MAX);
    big.addNum(INT_MAX - 1);
    check(fabs(big.findMedian() - 2147483646.5) < 1e-9, "big-values");
    return 0;
}

// follow-up (ideas only):
// - sliding window median (LC 480) -> two multisets instead of heaps: erase the leaving
//   value by iterator (find, never erase(value)), rebalance after erase and insert
// - all values in [0, 100] -> count[v] buckets; addNum is count[num]++, findMedian
//   walks the buckets to the middle rank(s), O(100) per query
// - 99% of values in [0, 100] -> the buckets plus counts of values below 0 and above
//   100 (shift the walk by the low count); keep the rare outliers in multisets
// - arbitrary k-th smallest query -> order-statistics tree, or two multisets rebalanced
//   so the lower one holds exactly k values (the answer is its max)
// - remove a value, keeping the heaps -> lazy deletion: a map of values to skip when
//   they reach a top, plus the live size of each half
