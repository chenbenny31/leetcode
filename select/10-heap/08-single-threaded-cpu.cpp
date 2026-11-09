// LC 1834 Single-Threaded CPU (Medium). Pattern: sort by arrival + min-heap simulation.
// tasks[i] = [enqueueTime, processingTime]. When idle, the CPU starts the available task
// with the shortest processing time (ties: smaller index). Return the order of indices.

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

// follow-up: k CPUs (LC 1882 Process Tasks Using Servers) -> second heap of busy
//   (freeTime, server); release servers whose freeTime <= time before assigning
