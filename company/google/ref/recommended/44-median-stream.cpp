// 44 · Find Median from Data Stream — LC 295, 480
// Mother: select/10-heap/04-median-stream.cpp
// (1) LC 295: addNum(num) from a stream; findMedian() = median so far
// (2) LC 480: median of every window of k values, 1 <= k <= n
// reported: LC 295, sliding window median ×2, trimmed average (45) ×5
//
// clarify:
// findMedian before any addNum -> assume not called
// dups -> count separately
// two middles overflow when added -> add as double
// even cnt -> mean of the two middles, as double

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

// (1) two heaps
// core idea: max-heap low = smaller half, min-heap high = larger half; median on the tops
// T: addNum O(log n), findMedian O(1), S: O(n)
class MedianFinder {
    // max-heap: smaller half, holds the extra value when cnt is odd
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

// (2) two multisets
// core idea: outgoing value must be erased from the middle -> multisets replace heaps
// low = smaller ceil(size / 2) values, high = the rest
// T: O(n log k), S: O(k)
vector<double> medianSlidingWindow(const vector<int>& nums, int k) {
    // smaller half of window, holds the extra value when k is odd; max = last element
    multiset<int> low;
    // larger half of window
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
        // add through low, as in addNum
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

// follow-up:
// window median w/ heaps -> lazy deletion: hash map cnts values to skip at a top
// values in [0, 100] -> cnt[v] buckets, walk for the middles; O(100) per query
// 99% of values in [0, 100] -> same buckets + cnts below / above, outliers in multisets
// arbitrary k-th smallest -> order-statistics tree, or two multisets w/ exactly k in low
