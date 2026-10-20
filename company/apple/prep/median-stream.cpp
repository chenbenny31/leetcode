// addNum() and findMedian()

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

// two heaps
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
    MedianFinderHeaps b;
    b.addNum(1);
    check(fabs(b.findMedian() - 1.0) < 1e-9, "one");
    b.addNum(2);
    check(fabs(b.findMedian() - 1.5) < 1e-9, "even");
    b.addNum(0);
    check(fabs(b.findMedian() - 1.0) < 1e-9, "odd");
}
