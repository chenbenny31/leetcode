// ByteDance (FastPrep, phone, team-lead round): single CPU, processes[i] = [arrival,
// burst], no preemption. When the CPU frees up, run the arrived process with the smallest
// burst (ties -> smaller index); if none has arrived, idle. Return indices in run order.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::priority_queue;
using std::greater;
using std::pair;

// Sweep in arrival order; push arrived processes into a min-heap of (burst, index) and
// run the top; empty heap -> jump the clock to the next arrival. T: O(n log n), S: O(n).
vector<int> scheduleProcesses(const vector<vector<int>>& processes) {
    int n = processes.size();
    // byArrival = process indices sorted by arrival time
    vector<int> byArrival(n);
    for (int i = 0; i < n; i++) {
        byArrival[i] = i;
    }
    sort(byArrival.begin(), byArrival.end(), [&](int a, int b) {
        return processes[a][0] < processes[b][0];
    });

    // arrived, not yet run, as (burst, index); smallest burst, then smallest index on top
    using Job = pair<int, int>;
    priority_queue<Job, vector<Job>, greater<Job>> waiting;
    vector<int> order;
    long long t = 0;
    // byArrival[next] = earliest process not yet pushed
    int next = 0;

    while ((int)order.size() < n) {
        while (next < n && processes[byArrival[next]][0] <= t) {
            int i = byArrival[next];
            waiting.push({processes[i][1], i});
            next++;
        }

        if (waiting.empty()) {
            // idle until the next process arrives
            t = processes[byArrival[next]][0];
            continue;
        }

        Job job = waiting.top();
        waiting.pop();
        order.push_back(job.second);
        t += job.first;
    }
    return order;
}

void test(const string& label, const vector<vector<int>>& processes,
          const vector<int>& want) {
    cout << label << (scheduleProcesses(processes) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("idle-gap", {{5, 2}, {0, 1}}, {1, 0});
    test("short-first", {{0, 4}, {1, 3}, {2, 1}}, {0, 2, 1});
    test("tie-index", {{0, 2}, {0, 2}, {0, 1}}, {2, 0, 1});
    test("single", {{7, 3}}, {0});
}

// follow-up: preemptive (SRTF) -> heap of (remaining, idx); run top until min(t + rem,
//   next arrival), push it back with the remainder
// follow-up: average waiting time -> wait_i = start_i - arrival_i, sum while popping
// follow-up: round robin, quantum q -> FIFO queue, run min(q, rem); enqueue arrivals up
//   to the slice end before re-queuing the current job
// follow-up: k CPUs -> extra min-heap of (freeTime, cpu); next start = top freeTime
