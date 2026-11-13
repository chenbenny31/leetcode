// 104 · Single-Threaded CPU — LC 1834
// Mother problem: select/optional/10-heap/08-single-threaded-cpu.cpp
//
// tasks[i] = [enqueueTime, processingTime]. When idle, the CPU starts the available task
// with the shortest processing time (ties: smaller index). Return the order of indices.
//
// Clarify
// - choice rule? → among arrived tasks, shortest processing time, ties by smaller index
// - idle CPU? → jumps to the next arrival
// - non-preemptive? → yes, a task runs to completion
// - overflow? → time sums can pass INT_MAX, use long long
//
// Core idea
// - Sort (enqueueTime, processingTime, index) by arrival.
// - Loop: if the heap is empty and time < next arrival, jump time; push every arrived
//   task into a min-heap of (processingTime, index); pop the top, time += its length.
// - T O(n log n), S O(n).
//
// Tests
// - shortest first {{0,5},{1,2},{1,1}} → {0,2,1}
// - tie by index {{0,3},{0,1},{0,1}} → {1,2,0}
// - idle gap {{10,1},{0,2}} → {1,0}
// - big times (two 1e9 tasks) → no overflow, {0,2,1}
//
// Follow-ups
// - k CPUs (LC 1882 process tasks using servers) → second heap of busy (freeTime,
//   server), release servers with freeTime <= time before assigning; see optional 122

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

// Walk tasks in arrival order; push everything that has arrived, run the heap top,
// jump the clock when nothing is waiting. T: O(n log n), S: O(n).
vector<int> getOrder(const vector<vector<int>>& tasks) {
    int n = tasks.size();
    // {enqueueTime, processingTime, index}, sorted by arrival
    vector<vector<int>> jobs;
    for (int i = 0; i < n; i++) { jobs.push_back({tasks[i][0], tasks[i][1], i}); }
    sort(jobs.begin(), jobs.end());
    // (processingTime, index) of arrived tasks; shortest, then smallest index on top
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
