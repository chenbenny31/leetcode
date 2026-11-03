// half-open or closed? e.g. [1,3], [3,5] conflict? no, [start, end)
// sorted? no
// empty? yes
// return?
// range of n and time? fit in int

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

// sort by start, T: O(nlogn), S: O(1)
bool canAttendMeetings(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] < intervals[i - 1][1]) {
            return false;
        }
    }
    return true;
}

// sort by start, keep minHeap of end times of rooms in use
// heap size is the need rooms, T: O(nlogn), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> ends; // top is min end
    for (const vector<int>& meeting : intervals) {
        if (!ends.empty() && ends.top() <= meeting[0]) { // re-use room
            ends.pop();
        }
        ends.push(meeting[1]);
    }
    return ends.size();
}

// follow-up: no heap, sweep line, T: O(nlogn), S: O(n)
int minMeetingRoomsSweep(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events;
    for (const vector<int>& intv : intervals) {
        events.push_back({intv[0], 1});
        events.push_back({intv[1], -1});
    }
    sort(events.begin(), events.end());
    int rooms = 0;
    int best = 0;
    for (const pair<int, int>& e : events) {
        rooms += e.second;
        best = max(best, rooms);
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
