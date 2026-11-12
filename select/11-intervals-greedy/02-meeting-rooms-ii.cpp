// LC 253 Meeting Rooms II (Medium). Pattern: sort by start + min-heap of end times.
// Meetings are [start, end); return the min number of rooms so every meeting has one.
// A room freed at time t can host a meeting starting at t.

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
using std::max;

// Sort by start; before placing a meeting free every room that has ended. The most
// rooms in use at once is the answer. T: O(n log n), S: O(n).
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // end times of the meetings currently holding a room, earliest on top
    priority_queue<int, vector<int>, greater<int>> ends;
    int best = 0;
    for (const auto& meeting : intervals) {
        // <=: a room freed at t is free for a start at t
        while (!ends.empty() && ends.top() <= meeting[0]) {
            ends.pop();
        }
        ends.push(meeting[1]);
        best = max(best, (int)ends.size());
    }
    return best;
}

// Sweep line: +1 at each start, -1 at each end; the max running sum is the answer.
// T: O(n log n), S: O(n).
int minRoomsSweep(const vector<vector<int>>& intervals) {
    // (time, +1 start / -1 end); pair order puts -1 first at equal times, so a room
    // freed at t is reused by a start at t
    vector<pair<int, int>> events;
    for (const auto& meeting : intervals) {
        events.push_back({meeting[0], 1});
        events.push_back({meeting[1], -1});
    }
    sort(events.begin(), events.end());
    int inUse = 0;
    int best = 0;
    for (const auto& event : events) {
        inUse += event.second;
        best = max(best, inUse);
    }
    return best;
}

// LC 252: sort by start; a meeting starting before the previous one ends is a clash.
// T: O(n log n), S: O(n).
bool canAttendAll(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < (int)intervals.size(); i++) {
        // [start, end): starting exactly at the previous end is fine
        if (intervals[i][0] < intervals[i - 1][1]) { return false; }
    }
    return true;
}

// J45: [start, end) in minutes on a 24-hour circle; end < start wraps past midnight,
// start == end is a full-day meeting. Split a wrapping meeting into [start, DAY) and
// [0, end), then sweep: its two pieces never overlap each other, so the max is
// unchanged. T: O(n log n), S: O(n).
int minRoomsCyclic(const vector<vector<int>>& meetings) {
    // minutes in a day
    const int DAY = 1440;
    // linear [start, end) pieces of all meetings, inside [0, DAY)
    vector<vector<int>> pieces;
    for (const auto& meeting : meetings) {
        int start = meeting[0];
        int end = meeting[1];
        if (start < end) {
            pieces.push_back({start, end});
        } else if (start == end) {
            pieces.push_back({0, DAY});
        } else {
            pieces.push_back({start, DAY});
            // end == 0: nothing after midnight; an empty piece's -1 would hide a +1
            if (end > 0) { pieces.push_back({0, end}); }
        }
    }
    return minRoomsSweep(pieces);
}

// Google (car rental): unsorted [start, end) requests; a car whose request ends at t can
// take one starting at t. Return {cars, carOf}, carOf[i] = car id (0..cars-1) of request
// i in input order. Go by start; reuse the car that frees up first if it is free by
// then, else open a new car. T: O(n log n), S: O(n).
pair<int, vector<int>> assignRooms(const vector<vector<int>>& intervals) {
    int n = intervals.size();
    // (start, request index), sorted by start
    vector<pair<int, int>> order;
    for (int i = 0; i < n; i++) { order.push_back({intervals[i][0], i}); }
    sort(order.begin(), order.end());
    // (end of its last request, car id) for every car, earliest end on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> cars;
    vector<int> carOf(n, -1);
    int count = 0;
    for (const auto& [start, idx] : order) {
        int car = count;
        if (!cars.empty() && cars.top().first <= start) {
            car = cars.top().second;
            cars.pop();
        } else {
            count++;
        }
        carOf[idx] = car;
        cars.push({intervals[idx][1], car});
    }
    return {count, carOf};
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    bool ok = minMeetingRooms(intervals) == want && minRoomsSweep(intervals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testAttend(const string& label, const vector<vector<int>>& intervals, bool want) {
    cout << label << (canAttendAll(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testCyclic(const string& label, const vector<vector<int>>& meetings, int want) {
    cout << label << (minRoomsCyclic(meetings) == want ? " PASS\n" : " FAIL\n");
}

void testAssign(const string& label, const vector<vector<int>>& intervals, int wantCount,
                const vector<int>& wantCarOf) {
    auto [count, carOf] = assignRooms(intervals);
    bool ok = count == wantCount && carOf == wantCarOf;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2);
    test("long-one", {{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 2);
    test("same-time", {{1, 4}, {1, 4}, {1, 4}}, 3);
    test("empty", {}, 0);
    testAttend("attend-back-to-back", {{5, 6}, {1, 5}}, true);
    testAttend("attend-unsorted-clash", {{6, 9}, {1, 3}, {2, 4}}, false);
    testAttend("attend-empty", {}, true);
    testCyclic("cyclic-wrap-overlap", {{1320, 120}, {60, 90}}, 2);
    testCyclic("cyclic-wrap-touch", {{1320, 60}, {60, 120}, {1200, 1320}}, 1);
    testCyclic("cyclic-full-day", {{600, 600}, {60, 120}}, 2);
    testCyclic("cyclic-end-midnight", {{1380, 0}, {0, 60}, {0, 60}}, 2);
    testAssign("assign-reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2, {0, 1, 0});
    testAssign("assign-unsorted", {{5, 7}, {1, 5}, {2, 6}}, 2, {0, 0, 1});
    testAssign("assign-same-time", {{1, 4}, {1, 4}}, 2, {0, 1});
    testAssign("assign-empty", {}, 0, {});
}

// follow-up: peak concurrency intervals -> sweep; record the time ranges where the
//   running count equals the max
