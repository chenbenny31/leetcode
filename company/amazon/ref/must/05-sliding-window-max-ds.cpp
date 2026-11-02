// SlidingWindow(k): put(x) adds x and drops the oldest value once more than k are held;
// getMax() returns the max of the values held (called only when non-empty).
// Use O(k) storage.

#include <iostream>
#include <string>
#include <deque>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::deque;
using std::pair;
using std::max;

// straightforward: keep the last k values in a queue and scan for the max.
// T: put O(1), getMax O(k). S: O(k).
class SlidingWindow {
    int k;
    // the last k values, oldest at the front
    deque<int> window;

public:
    SlidingWindow(int k) : k(k) {}

    void put(int x) {
        window.push_back(x);
        if ((int)window.size() > k) { window.pop_front(); }
    }

    int getMax() {
        int best = window.front();
        for (int x : window) { best = max(best, x); }
        return best;
    }
};

// improved: monotonic deque of (value, index), values strictly decreasing from front to
// back; a value with a newer, bigger value behind it can never be the max again.
// T: put O(1) amortized, getMax O(1). S: O(k).
class SlidingWindowDeque {
    int k;
    // number of values put so far; also the index of the next value
    int count = 0;
    // (value, index) candidates for the max, front = current max
    deque<pair<int, int>> dq;

public:
    SlidingWindowDeque(int k) : k(k) {}

    void put(int x) {
        while (!dq.empty() && dq.back().first <= x) { dq.pop_back(); }
        dq.push_back({x, count});
        // the front left the window once it is k or more positions old
        if (dq.front().second <= count - k) { dq.pop_front(); }
        count++;
    }

    int getMax() { return dq.front().first; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SlidingWindow a(3);
    SlidingWindowDeque b(3);
    a.put(5);
    b.put(5);
    check(a.getMax() == 5 && b.getMax() == 5, "one");
    for (int x : {1, 2}) {
        a.put(x);
        b.put(x);
    }
    check(a.getMax() == 5 && b.getMax() == 5, "full");
    a.put(3);
    b.put(3);
    check(a.getMax() == 3 && b.getMax() == 3, "evict-max");
    a.put(3);
    b.put(3);
    check(a.getMax() == 3 && b.getMax() == 3, "dup");
    SlidingWindow c(1);
    SlidingWindowDeque d(1);
    c.put(9);
    d.put(9);
    c.put(4);
    d.put(4);
    check(c.getMax() == 4 && d.getMax() == 4, "k-one");
}
