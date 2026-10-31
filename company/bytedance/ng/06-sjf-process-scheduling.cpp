// ByteDance (FastPrep, phone, team-lead round): single CPU, processes[i] = [arrival,
// burst]. The CPU starts at time 0, runs one process at a time with no preemption.
// When the CPU is free at time t, the waiting set is every process not yet run with
// arrival <= t; pick the smallest burst, ties -> smaller index. If nothing is waiting,
// stay idle until the next arrival. Return the indices in the order they run.
// (The report only said "process scheduling with a heap"; the rules are FastPrep's.)

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

// Walk processes in arrival order. Each time the CPU frees up, move everything that
// has arrived into a min-heap keyed by (burst, index) and run the top; if the heap is
// empty, jump the clock to the next arrival. T: O(n log n), S: O(n).
vector<int> scheduleProcesses(const vector<vector<int>>& processes) {
    int n = processes.size();
    // byArrival = indices sorted by arrival time (index breaks ties)
    vector<int> byArrival(n);
    for (int i = 0; i < n; i++) {
        byArrival[i] = i;
    }
    sort(byArrival.begin(), byArrival.end(), [&](int a, int b) {
        return processes[a][0] < processes[b][0];
    });

    // waiting processes as (burst, index); smallest burst, then smallest index on top
    using Job = pair<int, int>;
    priority_queue<Job, vector<Job>, greater<Job>> waiting;
    vector<int> order;
    long long t = 0;
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

        auto [burst, i] = waiting.top();
        waiting.pop();
        order.push_back(i);
        t += burst;
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
