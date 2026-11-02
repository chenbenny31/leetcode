// getMax call on empty window? no
// range of k? k >= 1
// valus -> neg, dup allowed
// oldest meaning -> order by put calls
// freq of put, getMax calls -> getMax O(k) vs O(1)
// mem limit -> O(k), no keep full history

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

// keep last k values in queue, scan for max
class SlidingWindow {
private:
    int k;
    deque<int> win; // S: O(k)

public:
    SlidingWindow(int k) : k(k) {}

    void put(int x) { // T: O(1)
        win.push_back(x);
        if ((int)win.size() > k) {
            win.pop_front();
        }
    }

    int getMax() { // T: O(k)
        int best = win.front();
        for (int v : win) {
            best = max(best, v);
        }
        return best;
    }
};

// mono-queue of (val, idx), strictly decre from front, idx for in-win check
class SlidingWindowDeque {
private:
    int k;
    int count = 0; // size; next idx
    deque<pair<int, int>> dq; // S: O(k)

public:
    SlidingWindowDeque(int k) : k(k) {}

    void put(int x) { // T: O(1) amortized 
        while (!dq.empty() && x >= dq.back().first) { // >=, if dup keep newest one for in-win
            dq.pop_back();
        }
        dq.push_back({x, count}); // then dq.front() is not empty
        if (dq.front().second <= count - k) { // out of win
            dq.pop_front();
        }
        count++;
    }

    int getMax() { // T: O(1)
        return dq.front().first;
    }
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
