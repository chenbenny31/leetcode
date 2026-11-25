// uniq starts?
// tie break by min room id

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

// 2 min-heaps, freed of room-ids, taken of (end, room-ids)
// T: O(mlogm + mlogn), S: O(n), m = meetings, n = rooms
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> freed; // top by avail room-id
    for (int i = 0; i < n; i++) {
        freed.push(i);
    }
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> taken; // top by (end-time, room-id)
    vector<int> count(n, 0); // count[room] = meetings this room hold

    for (const vector<int>& m : meetings) {
        long long start = m[0];
        long long dur = m[1] - start;
        while (!taken.empty() && taken.top().first <= start) {
            freed.push(taken.top().second);
            taken.pop();
        }

        if (!freed.empty()) {
            int room = freed.top();
            freed.pop();
            taken.push({start + dur, room});
            count[room]++;
        } else {
            auto [end, room] = taken.top();
            taken.pop();
            taken.push({end + dur, room});
            count[room]++;
        }
    }
    int best = 0;
    for (int i = 1; i < n; i++) {
        if (count[i] > count[best]) { best = i; }
    }
    return best;
}

// sort by starts, min-heap of finish times of CPUs in use
// all in use -> take one that frees first, refresh begin, T: O(nlogn), S: O(n)
int lastFinish(vector<int> starts, int execTime, int maxCPUs) {
    sort(starts.begin(), starts.end());
    priority_queue<int, vector<int>, greater<int>> taken; // min avail time on top
    int finish = 0;
    for (int start : starts) {
        int begin = start;
        if ((int)taken.size() == maxCPUs) {
            begin = max(start, taken.top());
            taken.pop();
        }
        finish = begin + execTime;
        taken.push(finish);
    }
    return finish;
}

struct Order {
    int id;
    int pickup;
    int ret;
};

// sort by pickup, re-use the car back first if avail to pickup, else add
// T: O(nlogn), S: O(n)
vector<vector<int>> assignOrders(vector<Order> orders) {
    sort(orders.begin(), orders.end(), [](const Order& a, const Order& b) {
        return a.pickup < b.pickup; // min pickup first
    });
    vector<vector<int>> cars;
    // (avail-time, car), top by min avail-time
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> taken;
    for (const Order& order : orders) {
        int car = cars.size();
        if (!taken.empty() && taken.top().first <= order.pickup) {
            car = taken.top().second;
            taken.pop();
        } else {
            cars.push_back({});
        }

        cars[car].push_back(order.id);
        taken.push({order.ret, car});
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
