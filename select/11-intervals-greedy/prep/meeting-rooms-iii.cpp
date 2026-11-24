// rooms 0..n-1, meetings [start, end) with uniq starts, assign lowest free room
// if none -> wait for frees first, keep dur, return most-used one

// on tie -> lowest room id

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

// 2 min-heaps: free-room-ids, (end, room-id) of busy ones, go by start
// free rooms ended by start, take lowest free room ,else delay into busy.top
// T: O(MlogM + Mlogn), S: O(n), M=meetings, n=rooms
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> free; // top by next free room id
    for (int i = 0; i < n; i++) { free.push(i); }
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> busy; // top by (min-end, room-id)
    vector<int> count(n, 0); // count[room] = meetings num in room-id

    for (const auto& m : meetings) {
        int start = m[0];
        int dur = m[1] - m[0];
        while (!busy.empty() && busy.top().first <= start) { // update buy with cur-timing = start
            free.push(busy.top().second);
            busy.pop();
        }

        if (!free.empty()) { // has avail free -> use top room in free
            int room = free.top();
            free.pop();
            busy.push({start + dur, room});
            count[room]++;
        } else { // non avail free -> use top room in busy
            auto [end, room] = busy.top();
            busy.pop();
            busy.push({end + dur, room});
            count[room]++;
        }
    }

    int best = 0;
    for (int room = 1; room < n; room++) {
        if (count[room] > count[best]) { best = room; }
    }
    return best;
}

// last finish with maxCPUs, sort starts, min-heap of finish times of CPUs in use
// all in use -> take one that ends first T: O(nlogn), S: O(n)
int lastFinish(vector<int> starts, int execTime, int maxCPUs) {
    sort(starts.begin(), starts.end());
    priority_queue<int, vector<int>, greater<int>> busy; // top by earliest time to free

    int finish = 0;
    for (int start : starts) {
        int begin = start;
        if ((int)busy.size() == maxCPUs) {
            begin = max(start, busy.top());
            busy.pop();
        }
        
        finish = begin + execTime; // one state to trace latest finish time
        busy.push(finish);
    }
    return finish;
}

// fewest cars, order per car
struct Order {
    int id;
    int pickup;
    int end;
};

// sort by pickup, re-use the case back first, else add, T: O(nlogn), S: O(n)
vector<vector<int>> assignOrders(vector<Order> orders) {
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        return a.pickup < b.pickup; // smaller pickup first
    });
    vector<vector<int>> cars; // cars[car-id] = order-id, in pickup order
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> busy; // top by (min-end-time, car-id)
    for (const auto& order : orders) {
        int car = cars.size(); // next avail car-id
        if (!busy.empty() && busy.top().first <= order.pickup) {
            car = busy.top().second;
            busy.pop();
        } else {
            cars.push_back({}); // add a slot for new car-id
        }
        cars[car].push_back(order.id);
        busy.push({order.end, car});
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

// non-uniq starts -> sort(start, input-idx)
// per-task exec time -> sort (start, execTime), finish=max(finish, begin+exec)
// at most k cars -> rej order when all busy
