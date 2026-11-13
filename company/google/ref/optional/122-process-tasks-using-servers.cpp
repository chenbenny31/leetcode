// LC 1882 Process Tasks Using Servers: servers[i] = weight, tasks[j] = duration; task j
// arrives at second j and waits in FIFO order. A task takes the free server with the
// smallest (weight, index); if none is free it waits for the earliest one to free up.
// Return the server index used by each task.
//
// Clarify:
// - a server freed exactly at second t is available at t
// - free times can exceed int -> long long
// Idea: freeHeap = (weight, index) of free servers; busyHeap = (free time, weight,
//   index) of busy servers. For task j: time = max(time, j); if nothing is free, jump
//   time to busyHeap's top; release every busy server with free time <= time; take
//   freeHeap's top and push (time + tasks[j], weight, index).
// Pitfalls:
// - release with a while (all servers freed by time), not an if
// - time never goes backwards: queued tasks start at the current time, not at j
// Tests to write: one server, two tasks; weight tie by index; all busy -> earliest
//   free wins over lighter; freed exactly at arrival.

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

// two min-heaps: free servers by (weight, index), busy servers by free time.
// T: O((n + m) log n) for n servers and m tasks, S: O(n + m).
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

// follow-up: single CPU, shortest job first (LC 1834) -> optional 104 (getOrder)
// follow-up: meeting rooms III (must 08) -> same two-heap pattern with rooms and
//   delayed meetings
