// LC 253 Meeting Rooms II (Medium) — min-heap of end times / sweep over starts and ends
// meetings [start, end); return fewest rooms so every meeting has one
// room freed at t can host a meeting starting at t
//
// clarify:
// end exclusive or inclusive -> exclusive
// input sorted? -> no, sort first
// empty list -> 0
// only the cnt, or which room each gets -> cnt; room ids = follow-up
// n -> up to ~1e5, O(n log n) ok

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

// solution 1: min-heap of end times; extends to "which room" questions
// core idea: go by start; heap = ends of meetings in progress; pop ends <= new start
// max heap size = answer
// T: O(n log n), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // end times of meetings holding a room, earliest on top
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

// solution 2: sweep line; shortest, gives concurrency over time
// core idea: sorted events, +1 at start, -1 at end; max running sum = answer
// T: O(n log n), S: O(n)
int minRoomsSweep(const vector<vector<int>>& intervals) {
    // (time, +1 start / -1 end); -1 sorts first at equal t, so a freed room is reused
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

// follow-up:
// can one person attend all (LC 252) -> sort by start; start < prev end = clash
// closed intervals: min chairs (Google), min groups (LC 2406) -> pop only ends < start
// which room / car each gets (Google car rental) -> heap of (end, id); reuse top or new
// smallest free chair id (LC 1942) -> 2nd min-heap of free ids; release before seating
// ranges where concurrency peaks (Google) -> sweep events grouped by time; merge ranges
// meetings on a 24-hour circle -> split a wrap into [start, DAY) + [0, end), then sweep
// meeting rooms III (LC 2402) -> free-id heap + (end, room) heap; delay if none free
