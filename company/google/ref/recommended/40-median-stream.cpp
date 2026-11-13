// 40 · Find Median from Data Stream — LC 295, 480
// Mother problem: select/10-heap/04-median-stream.cpp
//
// Google reports: LC 295, sliding window median ×2, sliding window trimmed average (41)
// ×5
//
// LC 295: addNum(num) adds a value from a stream; findMedian() returns the median so far
// (mean of the two middle values when the count is even).
//
// Clarify
// - findMedian before any addNum? → never called; duplicates allowed
// - any int → add the two middles as double (overflow); even count → mean as double
//
// Core idea
// - Max-heap `low` = smaller half, min-heap `high` = larger half; low has the same size
//   or one more. Route each value through low into high, then move one back if high is
//   bigger (pushing into the smaller heap without comparing breaks low <= high).
// - Median = low.top() (odd) or mean of both tops.
// - addNum O(log n), findMedian O(1), S O(n).
//
// Tests
// - one value; even count with a negative; duplicate middle; INT_MAX pair (overflow)
//
// Follow-ups
// - sliding window median (LC 480) → `medianSlidingWindow` below: two multisets, erase
//   one copy by iterator (or heaps + lazy deletion via a hash map of values to skip)
// - values in [0, 100] → count[v] buckets, walk them for the middles, O(100) per query;
//   99% in range → same buckets plus counts below / above, outliers in multisets
// - arbitrary k-th smallest → order-statistics tree, or two multisets with exactly k low

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

// LC 480: median of every window of k values, 1 <= k <= n. The value leaving the window
// must be erased from the middle, so two multisets replace the heaps: low = the smaller
// ceil(size / 2) values, high = the rest. T: O(n log k), S: O(k).
// Pitfalls: erase the leaving value by iterator (find), never erase(value); look for it
//   in low first, else it is in high; rebalance after both the erase and the insert.
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
