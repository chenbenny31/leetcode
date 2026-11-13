// 127 · Process Tasks Using Servers — LC 1882
// servers[i] = weight, tasks[j] = duration; task j arrives at second j, tasks wait FIFO
// task takes free server w/ min (weight, idx), else waits; return server per task
//
// clarify:
// server freed exactly at t -> available at t
// free times -> can exceed int, use long long
// tie on weight -> lower idx

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <tuple>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::tuple;
using std::get;
using std::greater;
using std::max;

// core idea: 2 min-heaps: free servers by (weight, idx), busy ones by free time
// per task: time = max(time, j); none free -> jump to earliest free time; release; take
// T: O((n + m) log n), S: O(n + m) — n servers, m tasks
vector<int> assignTasks(const vector<int>& servers, const vector<int>& tasks) {
    // (weight, index) of free servers, lightest then lowest index on top
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> freeHeap;
    for (int i = 0; i < (int)servers.size(); i++) { freeHeap.push({servers[i], i}); }
    // (free time, weight, index) of busy servers, earliest free time on top
    priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>,
                   greater<tuple<long long, int, int>>> busyHeap;
    vector<int> result(tasks.size());
    long long time = 0;
    for (int i = 0; i < (int)tasks.size(); i++) {
        time = max(time, (long long)i);
        if (freeHeap.empty()) { time = get<0>(busyHeap.top()); }
        while (!busyHeap.empty() && get<0>(busyHeap.top()) <= time) {
            auto [freeAt, weight, index] = busyHeap.top();
            busyHeap.pop();
            freeHeap.push({weight, index});
        }
        auto [weight, index] = freeHeap.top();
        freeHeap.pop();
        result[i] = index;
        busyHeap.push({time + tasks[i], weight, index});
    }
    return result;
}

void test(const string& label, const vector<int>& servers, const vector<int>& tasks,
          const vector<int>& want) {
    cout << label << (assignTasks(servers, tasks) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-server", {5}, {3, 1}, {0, 0});
    test("weight-tie", {3, 3, 2}, {3, 3}, {2, 0});
    test("earliest-free", {1, 2}, {5, 2, 1}, {0, 1, 1});
    test("freed-on-time", {1, 2}, {1, 1}, {0, 0});
}

// follow-up:
// single CPU, shortest job first (LC 1834) -> optional 104 (getOrder)
// meeting rooms III (must 08) -> same two-heap pattern w/ rooms and delayed meetings
