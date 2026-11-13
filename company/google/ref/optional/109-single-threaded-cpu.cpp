// 109 · Single-Threaded CPU — LC 1834
// Mother: select/optional/10-heap/08-single-threaded-cpu.cpp
// tasks[i] = [enqueueTime, processingTime]; idle CPU runs the shortest arrived task
// ties: smaller idx; return the order of indices
//
// clarify:
// tie rule -> smaller idx
// preemption -> none
// CPU idle, nothing waiting -> jump to the next arrival
// time sums -> can pass INT_MAX, long long

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::sort;

// core idea: walk tasks in arrival order; push all arrived into min-heap (proc time, idx)
// run the top; jump the clock when nothing is waiting
// T: O(n log n), S: O(n)
vector<int> getOrder(const vector<vector<int>>& tasks) {
    int n = tasks.size();
    // {enqueueTime, processingTime, idx}, sorted by arrival
    vector<vector<int>> jobs;
    for (int i = 0; i < n; i++) { jobs.push_back({tasks[i][0], tasks[i][1], i}); }
    sort(jobs.begin(), jobs.end());
    // (processingTime, idx) of arrived tasks; shortest, then smallest idx on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> ready;
    vector<int> order;
    // sums of processing times can pass INT_MAX
    long long time = 0;
    int next = 0;
    while ((int)order.size() < n) {
        // idle: jump to the next arrival instead of ticking
        if (ready.empty() && time < jobs[next][0]) { time = jobs[next][0]; }
        while (next < n && jobs[next][0] <= time) {
            ready.push({jobs[next][1], jobs[next][2]});
            next++;
        }
        auto [proc, idx] = ready.top();
        ready.pop();
        time += proc;
        order.push_back(idx);
    }
    return order;
}

void test(const string& label, const vector<vector<int>>& tasks,
          const vector<int>& want) {
    cout << label << (getOrder(tasks) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("shortest-first", {{0, 5}, {1, 2}, {1, 1}}, {0, 2, 1});
    test("tie-by-index", {{0, 3}, {0, 1}, {0, 1}}, {1, 2, 0});
    test("idle-gap", {{10, 1}, {0, 2}}, {1, 0});
    test("big-times", {{0, 1000000000}, {0, 1000000000}, {1, 1}}, {0, 2, 1});
}

// follow-up:
// k CPUs (LC 1882) -> 2nd heap of busy (freeTime, server); see optional 122
// LC 1882 assign step -> release servers w/ freeTime <= time before assigning
