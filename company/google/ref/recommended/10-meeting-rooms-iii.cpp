// Three heap scheduling parts (Google, interview report).
// (1) LC 2402 Meeting Rooms III: rooms 0..n-1, meetings [start, end) with unique starts.
//     Each meeting takes the lowest-numbered free room; if none is free it waits for the
//     room that frees first (lowest number on ties) and keeps its duration. Return the
//     room that held the most meetings, lowest number on ties.
// (2) CPU tasks: scheduled start times, one fixed execution time, at most maxCPUs at
//     once; a task waits for the earliest free CPU. Return when the last task finishes.
// (3) Orders [id, pickup, return]: a car is free again at its return time (pickup ==
//     return is fine). Use the fewest cars; return each car's order ids in pickup order.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;
using std::sort;
using std::max;

// (1) sort by start. Before each meeting, free every room that ended by its start; take
// the lowest free room, else delay the meeting into the room that frees first.
// T: O(M log M + M log n), S: O(n).
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    // ids of free rooms, lowest on top
    priority_queue<int, vector<int>, greater<int>> freeRooms;
    for (int room = 0; room < n; room++) { freeRooms.push(room); }
    // (end time, room) of busy rooms, earliest end then lowest room on top;
    // long long since delays can push end times past INT_MAX
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> busy;
    // count[room] = meetings held in room
    vector<int> count(n, 0);
    for (const vector<int>& meeting : meetings) {
        long long start = meeting[0];
        long long duration = meeting[1] - meeting[0];
        while (!busy.empty() && busy.top().first <= start) {
            freeRooms.push(busy.top().second);
            busy.pop();
        }
        if (!freeRooms.empty()) {
            int room = freeRooms.top();
            freeRooms.pop();
            busy.push({start + duration, room});
            count[room]++;
        } else {
            auto [end, room] = busy.top();
            busy.pop();
            busy.push({end + duration, room});
            count[room]++;
        }
    }
    int best = 0;
    for (int room = 1; room < n; room++) {
        if (count[room] > count[best]) { best = room; }
    }
    return best;
}

// (2) sort the start times; keep the finish times of the CPUs in use. When all maxCPUs
// are in use, the task takes the one that frees first. T: O(t log t), S: O(t).
int lastFinish(vector<int> starts, int execTime, int maxCPUs) {
    sort(starts.begin(), starts.end());
    // finish time of each CPU in use, earliest on top
    priority_queue<int, vector<int>, greater<int>> cpuFree;
    int finish = 0;
    for (int start : starts) {
        int begin = start;
        if ((int)cpuFree.size() == maxCPUs) {
            begin = max(start, cpuFree.top());
            cpuFree.pop();
        }
        finish = begin + execTime;
        cpuFree.push(finish);
    }
    return finish;
}

struct Order {
    int id;
    int pickup;
    int ret;
};

// (3) sort by pickup; reuse the car that comes back first if it is back by the pickup,
// else add a car. The number of cars equals the max overlap. T: O(n log n), S: O(n).
vector<vector<int>> assignOrders(vector<Order> orders) {
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        return a.pickup < b.pickup;
    });
    // cars[car] = order ids served by car, in pickup order
    vector<vector<int>> cars;
    // (return time of last order, car), earliest return on top
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> back;
    for (const Order& order : orders) {
        int car = cars.size();
        if (!back.empty() && back.top().first <= order.pickup) {
            car = back.top().second;
            back.pop();
        } else {
            cars.push_back({});
        }
        cars[car].push_back(order.id);
        back.push({order.ret, car});
    }
    return cars;
}

void testRooms(const string& label, int n, const vector<vector<int>>& meetings,
               int want) {
    cout << label << (mostBooked(n, meetings) == want ? " PASS\n" : " FAIL\n");
}

void testCpu(const string& label, const vector<int>& starts, int execTime, int maxCPUs,
             int want) {
    bool ok = lastFinish(starts, execTime, maxCPUs) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testCars(const string& label, const vector<Order>& orders,
              const vector<vector<int>>& want) {
    cout << label << (assignOrders(orders) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    testRooms("reuse-free", 2, {{0, 10}, {1, 2}, {3, 4}}, 1);
    testRooms("delay-tie", 2, {{0, 6}, {1, 3}, {2, 5}, {4, 5}}, 0);
    testRooms("lowest-free", 3, {{0, 1}, {2, 3}, {4, 5}}, 0);
    testRooms("big-times", 1, {{0, 1000000000}, {1, 1000000000}, {2, 1000000000}}, 0);

    testCpu("queue", {1, 2, 3}, 2, 1, 7);
    testCpu("parallel", {0, 0, 0}, 3, 3, 3);
    testCpu("wait-two", {0, 0, 0}, 2, 2, 4);
    testCpu("idle-gap", {10, 0}, 2, 1, 12);
    testCpu("no-tasks", {}, 2, 1, 0);

    testCars("chain", {{1, 0, 2}, {2, 2, 4}}, {{1, 2}});
    testCars("overlap", {{1, 0, 3}, {2, 1, 4}}, {{1}, {2}});
    testCars("first-back", {{1, 0, 5}, {2, 1, 3}, {3, 4, 6}}, {{1}, {2, 3}});
    testCars("unsorted", {{2, 3, 4}, {1, 0, 2}}, {{1, 2}});
    testCars("no-orders", {}, {});
}

// follow-up: (1) with non-unique starts -> sort by (start, input index); same heaps
// follow-up: (2) per-task execution times -> same heap, finish = max over all pushes
// follow-up: (3) at most k cars -> when the top is not back and cars.size() == k, the
//   order is rejected (collect its id, or throw for assignOrders(orders, k))
