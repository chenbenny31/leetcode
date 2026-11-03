// LC 295: addNum(num) adds an integer from a stream; findMedian() returns the median so
// far (mean of the two middle values when the count is even).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::lower_bound;
using std::fabs;

// straightforward: keep every value in a sorted vector; the median is the middle.
class MedianFinder {
    vector<int> sorted;   // all values so far, ascending

public:
    // binary search the slot, then insert (shifts the tail). T: O(n).
    void addNum(int num) {
        sorted.insert(lower_bound(sorted.begin(), sorted.end(), num), num);
    }

    // T: O(1).
    double findMedian() {
        int n = sorted.size();
        if (n % 2 == 1) { return sorted[n / 2]; }
        return (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
    }
};

// improved: two heaps split the values into a smaller and a larger half; the median
// sits on their tops.
class MedianFinderHeaps {
    priority_queue<int> low;                               // max-heap: smaller half
    priority_queue<int, vector<int>, greater<int>> high;   // min-heap: larger half

public:
    // Pass through low into high, then rebalance so low keeps the extra one.
    // T: O(log n).
    void addNum(int num) {
        // routing via low keeps every value in low <= every value in high
        low.push(num);
        high.push(low.top());
        low.pop();
        if (high.size() > low.size()) {
            low.push(high.top());
            high.pop();
        }
    }

    // T: O(1).
    double findMedian() {
        if (low.size() > high.size()) { return low.top(); }
        return (low.top() + high.top()) / 2.0;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MedianFinder a;
    MedianFinderHeaps b;
    a.addNum(1);
    b.addNum(1);
    check(fabs(a.findMedian() - 1.0) < 1e-9 && fabs(b.findMedian() - 1.0) < 1e-9, "one");
    a.addNum(2);
    b.addNum(2);
    check(fabs(a.findMedian() - 1.5) < 1e-9 && fabs(b.findMedian() - 1.5) < 1e-9, "even");
    a.addNum(0);
    b.addNum(0);
    check(fabs(a.findMedian() - 1.0) < 1e-9 && fabs(b.findMedian() - 1.0) < 1e-9, "odd");
}
