// Canary rollback check (ASE release safety: canary + automated rollback).
// requests[i] = {time, ok} sorted by time (ok = 1 success, 0 error). The window at time t
// holds the requests with time in (t - window, t]. Roll back at the first request time
// t where the window has at least minRequests requests and its error rate is strictly
// above maxErrorPercent. Return that t, or -1 if the canary stays healthy.

#include <iostream>
#include <vector>
#include <string>
#include <deque>

using std::cout;
using std::string;
using std::vector;
using std::deque;

// Sliding window over time: a deque holds the requests still inside the window plus a
// running error count. Integer check errors * 100 > maxErrorPercent * total avoids
// floating point. T: O(n), S: O(n) for the window.
int rollbackTime(const vector<vector<int>>& requests, int window, int minRequests,
                 int maxErrorPercent) {
    // requests inside (t - window, t], oldest first: {time, ok}
    deque<vector<int>> inWindow;
    int errors = 0;
    for (const vector<int>& req : requests) {
        int t = req[0];
        inWindow.push_back(req);
        if (req[1] == 0) { errors++; }
        // drop requests that left the window; <= because the window is open on the left
        while (inWindow.front()[0] <= t - window) {
            if (inWindow.front()[1] == 0) { errors--; }
            inWindow.pop_front();
        }
        int total = inWindow.size();
        if (total >= minRequests && errors * 100 > maxErrorPercent * total) {
            return t;
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& requests, int want) {
    // window 10s, need 4 requests, roll back above 50% errors
    cout << label << (rollbackTime(requests, 10, 4, 50) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("healthy", {{1, 1}, {2, 0}, {3, 1}, {4, 1}, {5, 0}}, -1);
    test("too-few-requests", {{1, 0}, {2, 0}, {3, 0}}, -1);
    test("rollback", {{1, 1}, {2, 0}, {3, 0}, {4, 0}}, 4);
    test("old-errors-expire",
         {{1, 0}, {2, 0}, {3, 0}, {12, 1}, {13, 1}, {14, 0}, {15, 1}}, -1);
}

// follow-up: compare against the baseline fleet -> roll back if canary rate > baseline
//   rate + margin; keep one window per group
// follow-up: progressive rollout 1% -> 5% -> 25% -> 100% -> advance a stage only after
//   the window stays healthy for the whole bake time
// follow-up: latency instead of errors -> p99 over the window: two heaps or a sorted
//   multiset with deletions (sliding window median pattern)
