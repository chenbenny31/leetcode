// LC 295 Find Median from Data Stream (Hard). Pattern: two heaps, lower/upper half.
// addNum(num) adds a value from a stream; findMedian() returns the median so far (mean
// of the two middle values when the count is even).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::fabs;

// Two heaps split the values into a smaller and a larger half; the median sits on
// their tops. addNum T: O(log n), findMedian T: O(1), S: O(n).
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
        return (low.top() + high.top()) / 2.0;
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
    mf.addNum(5);
    check(fabs(mf.findMedian() - 5.0) < 1e-9, "dup-middle");
}

// follow-up: all values in [0, 100] -> count array of 101 buckets, walk to the middle,
//   O(100) per query
// follow-up: sliding window median (LC 480) -> two multisets (or heaps + lazy deletion)
