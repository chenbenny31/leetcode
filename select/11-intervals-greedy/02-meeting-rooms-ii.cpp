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

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    bool ok = minMeetingRooms(intervals) == want && minRoomsSweep(intervals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testAttend(const string& label, const vector<vector<int>>& intervals, bool want) {
    cout << label << (canAttendAll(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2);
    test("long-one", {{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 2);
    test("same-time", {{1, 4}, {1, 4}, {1, 4}}, 3);
    test("empty", {}, 0);
    testAttend("attend-back-to-back", {{5, 6}, {1, 5}}, true);
    testAttend("attend-unsorted-clash", {{6, 9}, {1, 3}, {2, 4}}, false);
    testAttend("attend-empty", {}, true);
}
