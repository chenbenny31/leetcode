// LC 295 Find Median from Data Stream: addNum(num) adds an integer from a stream;
// findMedian() returns the median so far (mean of the two middle values if even count).
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
// their tops. T: addNum O(log n), findMedian O(1), S: O(n).
class MedianFinder {
    priority_queue<int> low;                               // smaller half, max on top
    priority_queue<int, vector<int>, greater<int>> high;   // larger half, min on top

public:
    void addNum(int num) {
        // routing through low keeps every value in low <= every value in high
        low.push(num);
        high.push(low.top());
        low.pop();
        // low keeps the extra element when the count is odd
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
    mf.addNum(2);
    check(fabs(mf.findMedian() - 3.5) < 1e-9, "even");
    mf.addNum(9);
    check(fabs(mf.findMedian() - 5.0) < 1e-9, "odd");
    mf.addNum(-4);
    mf.addNum(-4);
    check(fabs(mf.findMedian() - 2.0) < 1e-9, "negative-duplicates");
}
