// meetings [start, end), return min rooms to accommodate all, rooms freed at t can host meeting start at t

// end is exclusive -> t = end can place new meeting
// input is not sorted
// empty list -> 0
// return only the count
// n range -> 1e5, O(nlogn) ok

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

// min-heap of end times; go by start, heap = ends of meeting in-progress, pop ends <= new start, ans is max heap-size
// T: O(nlogn), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end()); // sort by starts
    priority_queue<int, vector<int>, greater<int>> ends;
    int best = 0;
    for (const auto& intv : intervals) {
        while (!ends.empty() && ends.top() <= intv[0]) { // room is free at now
            ends.pop();
        }
        ends.push(intv[1]);
        best = max(best, (int)ends.size());
    }
    return best;
}

// sweep line, sorted events: +1 start, -1 end; max running sum = ans, T: O(nlogn), S: O(n)
int minRoomsSweep(vector<vector<int>> intervals) {
    vector<pair<int, int>> events;
    for (const auto& intv : intervals) {
        events.push_back({intv[0], +1});
        events.push_back({intv[1], -1});
    }
    sort(events.begin(), events.end());

    int run = 0;
    int best = 0;
    for (auto& e : events) {
        run += e.second;
        best = max(best, run);
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

// closed intervals -> pop when ends < start
// assign with id -> heap of (end, id); re-use top or new id
// smallest free id -> 2nd min-heap of free ids; release
// range at peak -> sweep events by time; merge ranges
