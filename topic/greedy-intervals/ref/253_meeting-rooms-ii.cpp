// LC 253 Meeting Rooms II (premium): minimum rooms needed for meetings [start, end).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;
using std::sort;

// Sweep by start with a min-heap of end times: reuse the room that frees first if it
// is free by now, else open a new room. T: O(n log n), S: O(n).
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // end times of the rooms in use, earliest first
    priority_queue<int, vector<int>, greater<int>> ends;
    for (const auto& iv : intervals) {
        if (!ends.empty() && ends.top() <= iv[0]) { ends.pop(); }
        ends.push(iv[1]);
    }

    return ends.size();
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minMeetingRooms(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("back-to-back", {{0, 5}, {5, 10}}, 1);
    test("all-overlap", {{1, 4}, {2, 5}, {3, 6}}, 3);
    test("nested", {{1, 10}, {2, 3}, {4, 5}}, 2);
    test("empty", {}, 0);
}
