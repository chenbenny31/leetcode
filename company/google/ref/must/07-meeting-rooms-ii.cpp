// 07 · Meeting Rooms II — LC 253, 1942, 2406
// Mother problem: select/11-intervals-greedy/02-meeting-rooms-ii.cpp
// Google reports: car rental assignment ×14 (`assignRooms`), min chairs ×9, LC 253 ×3,
// combined schedule ×4 and roster merge shifts (09), min CPUs ×2, cyclic rooms ×2,
// peak concurrency ×2 (`peakRanges`), meeting rooms III (08) ×4
//
// LC 253: meetings [start, end); min rooms so every meeting has one.
// Clarify
// - end exclusive? → [start, end): a room freed at t hosts a start at t (pop ends <=
//   start); closed bounds (someone leaving at t still sits at t) pop only ends < start
// - unsorted → sort a copy; empty → 0; n ~1e5 → O(n log n)
// Core idea
// - Sort by start; a min-heap holds the end times of meetings in progress; pop every
//   end <= the start before placing; the largest heap size is the answer.
// - T O(n log n), S O(n).
// Tests: back-to-back share a room; one long over three short → 2; identical → n; empty.
//
// Follow-ups
// - sweep line → +1 at start, -1 at end, sort (time, delta) so ends come first; max sum
// - can one person attend all (LC 252) → sort; any start < previous end is a clash
// - car rental: which car each request gets → `assignRooms` below
// - time ranges of peak concurrency → `peakRanges` below
// - smallest free chair (LC 1942) → `smallestChair` below
// - 24-hour circle → split a wrapping meeting into [start, 1440) and [0, end), then count
// - min groups of closed intervals (LC 2406) → same as rooms, pop only ends < start
// - fixed number of rooms with delays → meeting rooms III (08)
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// Google (car rental): unsorted [start, end) requests; a car whose request ends at t can
// take one starting at t. Return {cars, carOf}, carOf[i] = car id (0..cars-1) of request
// i in input order. Go by start; reuse the car that frees up first if it is free by
// then, else open a new car. T: O(n log n), S: O(n).
// Pitfalls: sort (start, index) pairs, not the intervals, so carOf stays in input order;
//   the heap holds (end, car id) so a reused car keeps its id; count is the next new id.
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

// Google (peak concurrency ×2): the time ranges [from, to) where the number of meetings
// in progress is at its max, adjacent ranges merged. Sweep the events grouped by time:
// after every event at time t, the count holds until the next event time.
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

// LC 1942: times[i] = [arrival, leaving] of friend i (distinct arrivals); each arrival
// takes the smallest free chair id. Return targetFriend's chair. Go by arrival; first
// free every chair whose friend left by now. T: O(n log n), S: O(n).
// Pitfall: release every leave <= arrival BEFORE seating, else a freed id is skipped.
int smallestChair(const vector<vector<int>>& times, int targetFriend) {
    int n = times.size();
    // (arrival, friend), sorted by arrival
    vector<pair<int, int>> order;
    for (int i = 0; i < n; i++) { order.push_back({times[i][0], i}); }
    sort(order.begin(), order.end());
    // ids of chairs freed so far, smallest on top
    priority_queue<int, vector<int>, greater<int>> freeChairs;
    // (leaving time, chair id) of seated friends, earliest leave on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> taken;
    // next never-used chair id
    int nextChair = 0;
    for (const auto& [arrival, friendId] : order) {
        // <=: a chair freed at t is free for an arrival at t
        while (!taken.empty() && taken.top().first <= arrival) {
            freeChairs.push(taken.top().second);
            taken.pop();
        }
        int chair = nextChair;
        if (freeChairs.empty()) {
            nextChair++;
        } else {
            chair = freeChairs.top();
            freeChairs.pop();
        }
        if (friendId == targetFriend) { return chair; }
        taken.push({times[friendId][1], chair});
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minMeetingRooms(intervals) == want ? " PASS\n" : " FAIL\n");
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

void testChair(const string& label, const vector<vector<int>>& times, int targetFriend,
               int want) {
    cout << label << (smallestChair(times, targetFriend) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2);
    test("long-one", {{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 2);
    test("same-time", {{1, 4}, {1, 4}, {1, 4}}, 3);
    test("empty", {}, 0);
    testAssign("assign-reuse-at-end", {{1, 5}, {2, 6}, {5, 7}}, 2, {0, 1, 0});
    testAssign("assign-unsorted", {{5, 7}, {1, 5}, {2, 6}}, 2, {0, 0, 1});
    testAssign("assign-same-time", {{1, 4}, {1, 4}}, 2, {0, 1});
    testPeak("peak-merged", {{1, 5}, {2, 6}, {5, 7}}, {{2, 6}});
    testPeak("peak-two-ranges", {{1, 3}, {2, 4}, {5, 7}, {6, 8}}, {{2, 3}, {6, 7}});
    testPeak("peak-empty", {}, {});
    testChair("chair-smallest-free-id", {{1, 4}, {2, 3}, {5, 6}}, 2, 0);
    testChair("chair-leave-at-arrival", {{1, 3}, {3, 5}, {2, 4}}, 1, 0);
    testChair("chair-new-chair", {{1, 5}, {2, 6}, {3, 7}}, 2, 2);
}
