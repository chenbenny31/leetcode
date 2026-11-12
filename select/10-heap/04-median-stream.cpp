// LC 295 Find Median from Data Stream (Hard). Pattern: two heaps, lower/upper half.
// addNum(num) adds a value from a stream; findMedian() returns the median so far (mean
// of the two middle values when the count is even).
//
// Clarify:
// - findMedian before any addNum? -> never called (one value at least); else ask
// - value range -> any int: the sum of the two middle values can overflow, so add them
//   as double (LC keeps |num| <= 1e5, a general stream does not)
// - duplicates -> allowed; even count -> mean of the two middles, as a double
// - up to ~5e4 interleaved calls -> O(log n) add, O(1) median; sorting per query or
//   inserting into a sorted vector (O(n) per add) is the brute force
// Idea: a max-heap low holds the smaller half, a min-heap high the larger half; low has
//   the same size as high or one more. The median is low.top() (odd count) or the mean
//   of both tops. Each new value passes through low into high, so low <= high holds.
// Pitfalls:
// - pushing into whichever heap is smaller, without comparing, breaks low.top() <=
//   high.top() (5 into low, then 1 into high); route each value through low first
// - rebalance after every add so the sizes never differ by more than one
// - high needs greater<int>; (a + b) / 2 in int truncates and can overflow
// Tests to write: one value; even count with a negative; odd count; duplicate middle;
//   two huge values (overflow).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <iterator>
#include <cmath>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::fabs;
using std::multiset;
using std::prev;

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
        // double first: two large ints overflow when added as int
        return ((double)low.top() + high.top()) / 2.0;
    }
};

// follow-up (LC 480, Google ×2): median of every window of k values, 1 <= k <= n. The
// value leaving the window must be erased from the middle, so two multisets replace the
// heaps: low = the smaller ceil(size / 2) values, high = the rest.
// T: O(n log k), S: O(k).
// Clarify: output size n - k + 1; duplicates allowed; even k -> mean as a double.
// Pitfalls: erase the leaving value by iterator (find), never erase(value); look for it
//   in low first, else it is in high; rebalance after both the erase and the insert.
// Tests to write: a duplicate leaves while its twin stays; even k; k = 1; huge values.
vector<double> medianSlidingWindow(const vector<int>& nums, int k) {
    // smaller half of the window, holds the extra value when k is odd; max = last element
    multiset<int> low;
    // larger half of the window
    multiset<int> high;
    vector<double> result;
    int n = nums.size();
    for (int i = 0; i < n; i++) {
        if (i >= k) {
            int out = nums[i - k];
            // erase one copy by iterator: low.erase(out) would erase every copy
            auto it = low.find(out);
            if (it != low.end()) {
                low.erase(it);
            } else {
                high.erase(high.find(out));
            }
        }
        // add through low, as in addNum, so every value in low <= every value in high
        low.insert(nums[i]);
        auto lowMax = prev(low.end());
        high.insert(*lowMax);
        low.erase(lowMax);
        // one move is enough: high is at most two bigger here
        if (high.size() > low.size()) {
            low.insert(*high.begin());
            high.erase(high.begin());
        }
        if (i < k - 1) { continue; }
        if (k % 2 == 1) {
            result.push_back(*prev(low.end()));
        } else {
            result.push_back(((double)*prev(low.end()) + *high.begin()) / 2.0);
        }
    }
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testWindow(const string& label, const vector<int>& nums, int k,
                const vector<double>& want) {
    vector<double> got = medianSlidingWindow(nums, k);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        ok = fabs(got[i] - want[i]) < 1e-9;
    }
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
    MedianFinder big;
    big.addNum(INT_MAX);
    big.addNum(INT_MAX - 1);
    check(fabs(big.findMedian() - 2147483646.5) < 1e-9, "big-values");

    testWindow("window-dup-leaves", {2, 2, 9, 0}, 3, {2, 2});
    testWindow("window-even", {-1, 6, 2, 8, 3}, 4, {4, 4.5});
    testWindow("window-k-one", {5, -3}, 1, {5, -3});
    testWindow("window-big", {INT_MAX, INT_MAX - 1}, 2, {2147483646.5});
}

// follow-up: all values in [0, 100] -> count[v] = copies of v, total = values added;
//   addNum is count[num]++, findMedian walks the buckets, O(100) per query:
//     int seen = 0, lowMid = -1, highMid = -1;
//     for (int v = 0; v <= 100; v++) {
//         seen += count[v];
//         if (lowMid == -1 && seen > (total - 1) / 2) { lowMid = v; }
//         if (highMid == -1 && seen > total / 2) { highMid = v; }
//     }   // median = (lowMid + highMid) / 2.0
// follow-up: 99% of values in [0, 100] -> the buckets above plus a count of values < 0
//   and > 100 (shift the walk by the low count); keep the rare outliers in multisets
// follow-up: sliding window median (LC 480) -> medianSlidingWindow above (or heaps +
//   lazy deletion: a hash map of values to skip when they reach a top)
