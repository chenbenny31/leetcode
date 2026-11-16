// Temperature Monitor (Google, interview report). TemperatureMonitor(k), k >= 1.
// record(temp) adds an int reading; getAverage() / getMax() cover the last k readings
// fewer than k readings -> all of them
//
// clarify:
// window by count or by time -> count; time = follow-up
// query before any record -> assume never
// negative readings -> allowed
// average type, sum overflow -> double; sum in long long
// how often getMax is called -> often, want O(1)

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

// solution 1: scan; fine if getMax is rare
// core idea: keep the last k readings w/ a running sum, scan them for the max
// T: record O(1), getAverage O(1), getMax O(k), S: O(k)
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

// solution 2: monotonic deque; O(1) getMax
// core idea: deque of max candidates, front = max
// reading leaves the back once a later one is >= it, the front once out of the window
// T: O(1) per call, S: O(k) — amortized
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
        // front left the window once its idx <= count - k
        while (candidates.front().first <= count - k) { candidates.pop_front(); }
        count++;
    }

    double getAverage() { return (double)sum / window.size(); }

    int getMax() { return candidates.front().second; }
};

// after each record, compare both classes' average and max w/ want
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

// follow-up:
// window by elapsed time (last D days) -> record(time, temp); store (time, temp) in both
// time window eviction -> before answering, evict while front's time < now - D, fix sum
// getMin too -> second deque w/ values strictly increasing
// median of the window -> two multisets (low / high halves), O(log k) per call
