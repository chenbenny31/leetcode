// uniq starts -> else follow-up
// delayed meeting -> keep dur
// on tie -> lowest room-id
// size of time -> might need long long
// pickup == return -> re-use

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

// 2 min-heap, free of room-id, busy of (end-time, room-id), take lowest free room, else delay busy.top()
// T: O(mlogm + mlogn), S: O(n), m=meetings, n=rooms
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> free; // top by earlist free room-id
    for (int room = 0; room < n; room++) { free.push(room); }
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> busy; // top by (earlist-end-time, room-id)
    vector<int> count(n, 0); // count[room] = meetings hold at room
    for (const vector<int>& m : meetings) {
        int start = m[0];
        int dur = m[1] - m[0];

        while (!busy.empty() && busy.top().first <= start) {
            free.push(busy.top().second);
            busy.pop();
        }

        if (!free.empty()) { // use free top
            int room = free.top();
            free.pop();
            busy.push({start + dur, room});
            count[room]++;
        } else { // wait for next avail in busy
            auto [end, room] = busy.top();
            busy.pop();
            busy.push({end + dur, room});
            count[room]++;
        }
    }

    int most = 0;
    for (int room = 1; room < n; room++) {
        if (count[room] > count[most]) { most = room; }
    }
    return most;
}

// min-heap of finish time of CPUs in use, all in use -> take one frees first
// T: O(nlogn), S: O(n)
int lastFinish(vector<int> starts, int execTime, int maxCPUs) {
    sort(starts.begin(), starts.end());
    priority_queue<int, vector<int>, greater<int>> busy; // top by earlist end time of cpu
    int finish = 0;
    for (int start : starts) {
        int begin = start;
        if ((int)busy.size() == maxCPUs) {
            begin = max(start, busy.top());
            busy.pop();
        }
        finish = begin + execTime;
        busy.push(finish);
    }
    return finish;
}

struct Order {
    int id;
    int startTime;
    int endTime;
};

// sort by start time, re-use the car avail at cur start, else add new car, T: O(nlogn), S: O(n)
vector<vector<int>> assignOrders(vector<Order> orders) {
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        return a.startTime < b.startTime; // sort by ascend start
    });
    vector<vector<int>> cars; // cars[car-id] = [orders served by car-id,]
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> busy; // top by (earliest-endTime, car-id)
    for (const Order& o : orders) {
        int car = cars.size(); // next avail car-id
        if (!busy.empty() && busy.top().first <= o.startTime) { // re-use avail car -> lazy
            car = busy.top().second;
            busy.pop();
        } else {
            cars.push_back({}); // create a new car-id
        }
        cars[car].push_back(o.id);
        busy.push({o.endTime, car});
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
    testCpu("wait-two", {0, 0, 0}, 2, 2, 4);
    testCpu("idle-gap", {10, 0}, 2, 1, 12);
    testCpu("no-tasks", {}, 2, 1, 0);

    testCars("chain", {{1, 0, 2}, {2, 2, 4}}, {{1, 2}});
    testCars("overlap", {{1, 0, 3}, {2, 1, 4}}, {{1}, {2}});
    testCars("first-back", {{1, 0, 5}, {2, 1, 3}, {3, 4, 6}}, {{1}, {2, 3}});
    testCars("unsorted", {{2, 3, 4}, {1, 0, 2}}, {{1, 2}});
}

// re-use, on-time, empty, overlap, chain

// non-uniq starts -> sort (start, idx) pairs heap
// per-task diff exec time -> sort (start, exec) pairs, finish = max(finish, begin + exec)
