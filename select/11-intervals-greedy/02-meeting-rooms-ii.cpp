// LC 253 Meeting Rooms II (Medium) — min-heap of end times / sweep over starts and ends
// Meetings are [start, end); return the fewest rooms so that every meeting has one.
// A room freed at time t can host a meeting that starts at t.
// Clarify: end exclusive or inclusive; input sorted or not; empty list; only the count
//   or also which room each meeting gets; n up to ~1e5.

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

// Core idea: go by start; a min-heap holds the end times of meetings in progress, and
// every end <= the new start is popped first. The largest heap size is the answer.
// T: O(n log n), S: O(n).
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

// Core idea: sweep the sorted start and end events, +1 at a start and -1 at an end; the
// largest running sum is the answer. T: O(n log n), S: O(n).
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

// follow-up (ideas only):
// - can one person attend all (LC 252) -> sort by start; a start before the previous end
//   is a clash
// - closed intervals: min chairs (Google), min groups (LC 2406) -> the same count, but
//   touching is an overlap: pop only ends < start, and +1 before -1 at equal times
// - which room or car each request gets (Google car rental) -> sort (start, index); heap
//   of (end, id): reuse the top if its end <= start, else open a new id
// - smallest free chair id (LC 1942) -> a second min-heap of free ids; release every
//   leave <= arrival BEFORE seating, then take the smallest id or a new one
// - time ranges where concurrency peaks (Google) -> sweep events grouped by time; the
//   count after time t holds until the next event time; merge adjacent ranges
// - meetings on a 24-hour circle -> split one that wraps midnight into [start, DAY) and
//   [0, end), a full-day one into [0, DAY), then sweep
// - fixed number of rooms, delays, most used room (LC 2402 meeting rooms III) -> heap of
//   free room ids plus heap of (end, room); delay a meeting to the earliest end
