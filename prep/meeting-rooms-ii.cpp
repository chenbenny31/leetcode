// end exclusive -> end <= curStart
// input not sort
// empty list -> 0
// return only count -> room-ids is follow-up
// n up to 1e5, O(nlogn) allowed

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
using std::pair;
using std::priority_queue;
using std::greater;
using std::sort;
using std::max;

// min-heap of end times, heap = ends of meetings running, pop ends <= new start, T: O(nlogn), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> ends;
    int rooms = 0;
    for (const auto& m : intervals) {
        while (!ends.empty() && ends.top() <= m[0]) {
            ends.pop();
        }
        ends.push(m[1]);
        rooms = max(rooms, (int)ends.size());
    }
    return rooms;
}

// sweep line, sorted events: start+1, end-1, max running = ans, T: O(nlogn), S: O(n)
int minRoomsSweep(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;
    for (const auto& m : intervals) {
        events.push_back({m[0], +1});
        events.push_back({m[1], -1});
    }
    sort(events.begin(), events.end());
    int used = 0;
    int rooms = 0;
    for (const auto& e : events) {
        used += e.second;
        rooms = max(rooms, used);
    }
    return rooms;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    bool ok = minMeetingRooms(intervals) == want && minRoomsSweep(intervals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2);
    test("long-one", {{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 2);
    test("same-time", {{1, 4}, {1, 4}, {1, 4}}, 3);
    test("empty", {}, 0);
    return 0;
}

// re-use, same-time, long-interval, empty-interval

// closed end -> pop only ends < start
// which id -> heap of (end, id), reuse top or new
// min ids -> 2 min-heap, free/busy ids, release before use
// range of peak concur -> sweep-line + merge range
