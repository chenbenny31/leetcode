// LC 252 / 253: meetings are half-open [start, end), so one ending at 10 and another
// starting at 10 do not clash.
// 252: can one person attend them all (no overlap)?  253: min rooms to hold them all.
// PointOne angle: detecting double-billed time; max concurrent entries.
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

// 252: after sorting by start, only neighbours can overlap. T: O(n log n), S: O(1).
bool canAttendAll(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    int n = intervals.size();
    for (int i = 1; i < n; i++) {
        if (intervals[i][0] < intervals[i - 1][1]) { return false; }
    }
    return true;
}

// 253: sweep by start; a min-heap holds the end times of rooms in use, and a meeting
// reuses the room that frees up first. T: O(n log n), S: O(n).
int minRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> ends;   // end times of busy rooms
    for (const auto& cur : intervals) {
        // earliest room is free by now (half-open: end == start is fine)
        if (!ends.empty() && ends.top() <= cur[0]) { ends.pop(); }
        ends.push(cur[1]);
    }
    return ends.size();
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    check(canAttendAll({{5, 10}, {0, 5}}), "attend-touch");
    check(!canAttendAll({{0, 6}, {5, 10}}), "attend-clash");
    check(minRooms({{0, 10}, {5, 15}, {10, 20}}) == 2, "rooms-two");
    check(minRooms({{0, 5}, {5, 10}}) == 1 && minRooms({}) == 0, "rooms-chain");
}
