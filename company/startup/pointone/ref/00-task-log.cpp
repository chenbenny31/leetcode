// One task is active at a time. start(id, t) starts task id and closes the active task
// (if any) at t; end(t) closes the active task (ignored if none). getSummary(t1, t2)
// returns the total time of closed (not active) tasks inside [t1, t2).
// Follow-up: return each id's own total as a map.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::max;
using std::min;

struct Task {
    int id;
    int start;
    // -1 while the task is active
    int end;
};

class TaskLog {
    // tasks in start order; they never overlap, so closed ends are sorted too
    vector<Task> tasks;

    // number of closed tasks: only the last one can still be active
    int closedCount() {
        int n = tasks.size();
        if (n > 0 && tasks[n - 1].end == -1) { return n - 1; }
        return n;
    }

public:
    void start(int id, int t) {
        end(t);
        tasks.push_back({id, t, -1});
    }

    void end(int t) {
        if (!tasks.empty() && tasks.back().end == -1) { tasks.back().end = t; }
    }

    // straightforward: clip every closed task to [t1, t2). T: O(n).
    int getSummary(int t1, int t2) {
        int total = 0;
        int n = closedCount();
        for (int i = 0; i < n; i++) {
            int lo = max(tasks[i].start, t1), hi = min(tasks[i].end, t2);
            if (lo < hi) { total += hi - lo; }
        }
        return total;
    }

    // improved: binary search the first closed task ending after t1, then walk until
    // tasks start at or after t2. T: O(log n + k) for k tasks in the window.
    int getSummaryFast(int t1, int t2) {
        int n = closedCount();
        int lo = 0, hi = n;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (tasks[mid].end <= t1) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        int total = 0;
        for (int i = lo; i < n && tasks[i].start < t2; i++) {
            total += min(tasks[i].end, t2) - max(tasks[i].start, t1);
        }
        return total;
    }

    // follow-up: same clipping, summed per id. T: O(n log u) for u distinct ids.
    map<int, int> getSummaryById(int t1, int t2) {
        map<int, int> result;   // result[id] = closed time of id inside [t1, t2)
        int n = closedCount();
        for (int i = 0; i < n; i++) {
            int lo = max(tasks[i].start, t1), hi = min(tasks[i].end, t2);
            if (lo < hi) { result[tasks[i].id] += hi - lo; }
        }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TaskLog log;
    log.start(1, 0);
    log.start(2, 10);
    log.end(15);
    log.end(17);
    log.start(1, 20);
    check(log.getSummary(0, 30) == 15 && log.getSummaryFast(0, 30) == 15, "all");
    check(log.getSummary(5, 12) == 7 && log.getSummaryFast(5, 12) == 7, "clip");
    check(log.getSummary(15, 20) == 0 && log.getSummaryFast(15, 20) == 0, "gap");
}
