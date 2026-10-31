// ByteDance (FastPrep, phone; the HM round asked both): intervals[i] = [start, end]
// occupies [start, end). A meeting ending at t and one starting at t do not overlap.
// Meeting Rooms (LC 252): can one person attend every meeting?
// Meeting Rooms II (LC 253): the minimum number of rooms so every meeting has one; a
// room freed at t can host a meeting starting at t.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::priority_queue;
using std::greater;
using std::max;

// Sort by start; any meeting that starts before the previous one ends is a conflict.
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

// Sort by start and keep a min-heap of end times of rooms in use. Before placing a
// meeting, free every room that ended by its start; the heap size is the rooms in use.
// T: O(n log n), S: O(n).
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // end times of the meetings currently holding a room
    priority_queue<int, vector<int>, greater<int>> ends;
    int best = 0;
    for (const auto& meeting : intervals) {
        while (!ends.empty() && ends.top() <= meeting[0]) {
            ends.pop();
        }
        ends.push(meeting[1]);
        best = max(best, (int)ends.size());
    }
    return best;
}

void testAttend(const string& label, const vector<vector<int>>& intervals, bool want) {
    cout << label << (canAttendMeetings(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testRooms(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minMeetingRooms(intervals) == want ? " PASS\n" : " FAIL\n");
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
