// LC 295 Find Median from Data Stream (Hard) — two heaps: lower half / upper half
// addNum(num) adds a stream value; findMedian() = median so far
// even cnt -> mean of the two middle values
//
// clarify:
// findMedian before any addNum -> assume never
// value range -> two middles can overflow int, add as double
// dup -> ok
// num of calls -> ~5e4, so O(log n) add, O(1) median

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

// core idea: max-heap low = smaller half, min-heap high = larger half; median on the tops
// invariant: low.size() == high.size() or one more
// T: O(log n) addNum, O(1) findMedian, S: O(n)
class MedianFinder {
    // max-heap: smaller half; holds the extra value when cnt is odd
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

// follow-up:
// sliding window median (LC 480) -> two multisets; erase leaving value by iterator
// all values in [0, 100] -> cnt[v] buckets; findMedian walks to the middle rank, O(100)
// 99% of values in [0, 100] -> buckets + multisets for the rare outliers below / above
// arbitrary k-th smallest query -> order-statistics tree, or lower multiset of exactly k
// remove a value, keeping heaps -> lazy deletion: skip map + live size of each half
