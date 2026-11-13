// Temperature Monitor (Google, interview report). TemperatureMonitor(k), k >= 1.
// record(temp) adds an integer reading. getAverage() and getMax() cover the last k
// readings (all of them if fewer than k exist). Assume both queries are called only after
// at least one record; getAverage returns a double.
//
// Clarify:
// - window = last k readings by count (stated); all readings while fewer than k exist
// - query before any record? -> never (stated); readings may be negative
// - average as double; the running sum in long long (k large ints overflow int)
// - many getMax calls? -> make getMax O(1): monotonic deque
// Idea: a queue of the last k readings with a running sum gives the average. For the
//   max, a deque of (index, value) with values strictly decreasing: a reading leaves the
//   back once a later reading is at least as hot, and the front once it leaves the
//   window, so the front is always the max.
// Pitfalls:
// - pop the back while value <= temp (an older equal reading is never needed again)
// - evict the front by index (index <= count - k), not by value
// - subtract the leaving reading from sum when the window exceeds k
// Tests to write: the max leaves the window; equal maxima; negatives; k = 1.

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::deque;
using std::queue;
using std::pair;
using std::max;

// straightforward: keep the window and scan it for the max.
// T: record O(1), getAverage O(1), getMax O(k), S: O(k).
class TemperatureMonitorScan {
    int k;
    // last k readings, oldest in front
    deque<int> window;
    // sum of the readings in window
    long long sum = 0;

public:
    TemperatureMonitorScan(int k) : k(k) {}

    void record(int temp) {
        window.push_back(temp);
        sum += temp;
        if ((int)window.size() > k) {
            sum -= window.front();
            window.pop_front();
        }
    }

    double getAverage() { return (double)sum / window.size(); }

    int getMax() {
        int best = window.front();
        for (int temp : window) { best = max(best, temp); }
        return best;
    }
};

// improved: monotonic deque of max candidates; a reading is dropped once a later reading
// is at least as hot, since it can never be the max again. T: O(1) amortized per call,
// S: O(k).
class TemperatureMonitor {
    int k;
    // last k readings, oldest in front
    queue<int> window;
    // sum of the readings in window
    long long sum = 0;
    // (index, value) max candidates, values strictly decreasing; front = window max
    deque<pair<int, int>> candidates;
    // index the next reading gets
    int count = 0;

public:
    TemperatureMonitor(int k) : k(k) {}

    void record(int temp) {
        window.push(temp);
        sum += temp;
        if ((int)window.size() > k) {
            sum -= window.front();
            window.pop();
        }
        while (!candidates.empty() && candidates.back().second <= temp) {
            candidates.pop_back();
        }
        candidates.push_back({count, temp});
        // the front left the window once its index is at most count - k
        while (candidates.front().first <= count - k) { candidates.pop_front(); }
        count++;
    }

    double getAverage() { return (double)sum / window.size(); }

    int getMax() { return candidates.front().second; }
};

// after each record, both classes' average and max are compared with want
void test(const string& label, int k, const vector<int>& temps,
          const vector<double>& wantAverage, const vector<int>& wantMax) {
    TemperatureMonitorScan scan(k);
    TemperatureMonitor fast(k);
    bool ok = true;
    for (int i = 0; i < (int)temps.size(); i++) {
        scan.record(temps[i]);
        fast.record(temps[i]);
        ok = ok && scan.getAverage() == wantAverage[i] && scan.getMax() == wantMax[i];
        ok = ok && fast.getAverage() == wantAverage[i] && fast.getMax() == wantMax[i];
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("max-leaves", 3, {21, 19, 23, 18, 22, 20},
         {21, 20, 21, 20, 21, 20}, {21, 21, 23, 23, 23, 22});
    test("equal-max", 2, {24, 24, 21}, {24, 24, 22.5}, {24, 24, 24});
    test("below-zero", 2, {-5, -3, -4}, {-5, -4, -3.5}, {-5, -3, -3});
    test("k-one", 1, {18, 25, 20}, {18, 25, 20}, {18, 25, 20});
}

// follow-up: window by elapsed time (last D days) instead of count -> record(time, temp);
//   store (time, temp) in window and candidates; before answering, evict while
//   front time < now - D (sum updated on window eviction)
// follow-up: getMin too -> a second deque with values strictly increasing
// follow-up: median of the window -> two multisets (low / high halves), O(log k) per call
