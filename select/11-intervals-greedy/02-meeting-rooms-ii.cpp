// LC 253 Meeting Rooms II (Medium). Pattern: sort by start + min-heap of end times.
// Meetings are [start, end); return the min number of rooms so every meeting has one.
// A room freed at time t can host a meeting starting at t.
//
// Clarify:
// - is the end exclusive? -> [start, end): a room freed at t hosts a start at t. If it is
//   closed [start, end] (chairs: someone leaving at t still sits at t), pop only ends
//   < start, and in the sweep put +1 before -1 at equal times
// - unsorted input -> sort a copy (taken by value here)
// - empty list -> 0
// - only the count, or which room each meeting gets? (car rental: assignRooms below)
// - n up to ~1e5 -> O(n log n)
// Idea: sort by start; a min-heap holds the end times of meetings in progress. Before
//   placing a meeting, pop every end <= its start (those rooms are free again). The
//   largest heap size seen = the most meetings in progress at once = the answer.
// Pitfalls:
// - <= vs < when popping decides whether back-to-back meetings share a room
// - min-heap: priority_queue<int, vector<int>, greater<int>>
// - sweep version: at equal times -1 must come before +1 (pair order does it)
// - empty input: return 0 before touching intervals[0] if you index anything
// Tests to write: back-to-back share a room; one long meeting over three short ones
//   -> 2; identical meetings -> n; empty -> 0.

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
// Clarify: ids are 0..cars-1 in order of first use; equal starts -> by request index;
//   several free cars -> this reuses the one freed first (if "smallest free id" is
//   asked, keep a second min-heap of free ids, as in meeting rooms III, G10).
// Pitfalls: sort (start, index) pairs, not the intervals, so carOf stays in input order;
//   the heap holds (end, car id) so a reused car keeps its id; count is the next new id.
// Tests to write: back-to-back reuse; unsorted input; same start twice; empty.
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

// follow-up (Google: peak concurrency ×2): the time ranges [from, to) where the number of
// meetings in progress is at its max, adjacent ranges merged. Sweep the events grouped
// by time: after every event at time t, the count holds until the next event time.
// T: O(n log n), S: O(n).
vector<pair<int, int>> peakRanges(const vector<vector<int>>& intervals) {
    // (time, +1 start / -1 end); ends first at equal times
    vector<pair<int, int>> events;
    for (const auto& meeting : intervals) {
        events.push_back({meeting[0], 1});
        events.push_back({meeting[1], -1});
    }
    sort(events.begin(), events.end());
    int numEvents = events.size();
    int inUse = 0;
    int best = 0;
    // [from, to) ranges where inUse == best
    vector<pair<int, int>> ranges;
    int i = 0;
    while (i < numEvents) {
        int t = events[i].first;
        while (i < numEvents && events[i].first == t) {
            inUse += events[i].second;
            i++;
        }
        // after the last event nothing is in progress
        if (i == numEvents) { break; }
        int nextT = events[i].first;
        if (inUse > best) {
            best = inUse;
            ranges.clear();
        }
        if (inUse == 0 || inUse < best) { continue; }
        if (!ranges.empty() && ranges.back().second == t) {
            ranges.back().second = nextT;
        } else {
            ranges.push_back({t, nextT});
        }
    }
    return ranges;
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

void testPeak(const string& label, const vector<vector<int>>& intervals,
              const vector<pair<int, int>>& want) {
    cout << label << (peakRanges(intervals) == want ? " PASS\n" : " FAIL\n");
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
    testPeak("peak-merged", {{1, 5}, {2, 6}, {5, 7}}, {{2, 6}});
    testPeak("peak-two-ranges", {{1, 3}, {2, 4}, {5, 7}, {6, 8}}, {{2, 3}, {6, 7}});
    testPeak("peak-empty", {}, {});
}

// follow-up: peak concurrency ranges (Google ×2) -> peakRanges above
// follow-up: min chairs (Google ×9) -> the same count; ask whether someone leaving at t
//   frees the chair for someone arriving at t ([start, end): pop <=) or not (pop <)
// follow-up: a fixed number of rooms, delays, which room is used most -> meeting rooms
//   III (company/google/ref/recommended/10-meeting-rooms-iii.cpp)
