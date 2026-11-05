// ByteDance (FastPrep, phone; HM round asked both): intervals[i] = [start, end), so a
// meeting ending at t and one starting at t do not overlap.
// LC 252: can one person attend every meeting?
// LC 253: min rooms so every meeting has one (a room freed at t can host a start at t).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::priority_queue;
using std::greater;
using std::max;
using std::pair;

// Sort by start; a meeting starting before the previous one ends is a conflict.
// T: O(n log n), S: O(1) besides the sort.
bool canAttendMeetings(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < intervals[i - 1][1]) {
            return false;
        }
    }
    return true;
}

// Sort by start; min-heap of end times of rooms in use, freed before each placement;
// the max heap size is the answer. T: O(n log n), S: O(n).
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // end times of the meetings currently holding a room
    priority_queue<int, vector<int>, greater<int>> ends;
    int best = 0;
    for (const vector<int>& meeting : intervals) {
        // while, not if: several rooms may have freed by this start
        while (!ends.empty() && ends.top() <= meeting[0]) {
            ends.pop();
        }
        ends.push(meeting[1]);
        best = max(best, (int)ends.size());
    }
    return best;
}

// Follow-up, sweep line: +1 at each start, -1 at each end; sort events, the max running
// count is the answer. T: O(n log n), S: O(n).
int minMeetingRoomsSweep(const vector<vector<int>>& intervals) {
    // (time, +1 for a start / -1 for an end); at equal times -1 sorts first, so a room
    // freed at t is reused by a meeting starting at t
    vector<pair<int, int>> events;
    for (const vector<int>& meeting : intervals) {
        events.push_back({meeting[0], 1});
        events.push_back({meeting[1], -1});
    }
    sort(events.begin(), events.end());
    int inUse = 0;
    int best = 0;
    for (const pair<int, int>& event : events) {
        inUse += event.second;
        best = max(best, inUse);
    }
    return best;
}

void testAttend(const string& label, const vector<vector<int>>& intervals, bool want) {
    cout << label << (canAttendMeetings(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testRooms(const string& label, const vector<vector<int>>& intervals, int want) {
    bool ok = minMeetingRooms(intervals) == want;
    bool okSweep = minMeetingRoomsSweep(intervals) == want;
    cout << label << (ok && okSweep ? " PASS\n" : " FAIL\n");
}

int main() {
    testAttend("touching", {{1, 3}, {3, 5}}, true);
    testAttend("nested", {{1, 5}, {2, 3}}, false);
    testAttend("unsorted", {{6, 8}, {1, 2}, {2, 6}}, true);
    testRooms("reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2);
    testRooms("long-one", {{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 2);
    testRooms("same-time", {{1, 4}, {1, 4}, {1, 4}}, 3);
    testRooms("single", {{0, 1}}, 1);
}

// follow-up: sweep line -> see minMeetingRoomsSweep() above; +1 / -1 events, -1 first
//   at equal times
// follow-up: which room each meeting gets (LC 2402) -> busy heap (end, roomId) + free
//   min-heap of ids; release ended rooms, else wait for earliest end, keep duration
// follow-up: heap with if instead of while -> pop at most one per meeting; heap never
//   shrinks, so answer = ends.size() at the end
// follow-up: online bookings (LC 731/732) -> map<int, int> diff, +1 start / -1 end, scan
//   prefix O(n) per booking; or dynamic segment tree with lazy max, O(log C)
