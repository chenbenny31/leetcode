// 07 · Meeting Rooms II — LC 253, 1942, 2406
// Mother: select/11-intervals-greedy/02-meeting-rooms-ii.cpp
// (1) LC 253: meetings [start, end); min rooms so every meeting has one
// (2) car rental: fewest cars for [start, end) requests + car id of each request
// (3) peak concurrency: time ranges where the most meetings are in progress
// (4) LC 1942: each arriving friend takes the smallest free chair; targetFriend's chair
//
// clarify:
// end exclusive or inclusive -> exclusive: room freed at t can host a start at t
// input unsorted -> sort by start
// empty input -> 0 rooms / no ranges
// (4) arrival times -> distinct

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

// (1) min rooms (LC 253)
// core idea: sort by start; min-heap of end times of meetings in progress
// pop every end <= start before placing; max heap size = answer
// T: O(n log n), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // ends = end times of meetings holding a room, earliest on top
    priority_queue<int, vector<int>, greater<int>> ends;
    int best = 0;
    for (const auto& meeting : intervals) {
        // <=: room freed at t is free for a start at t
        while (!ends.empty() && ends.top() <= meeting[0]) {
            ends.pop();
        }
        ends.push(meeting[1]);
        best = max(best, (int)ends.size());
    }
    return best;
}

// (2) fewest cars + car id per request
// core idea: go by start; reuse the car that frees first if free by then, else new car
// heap holds (end, car id) -> a reused car keeps its id
// T: O(n log n), S: O(n)
pair<int, vector<int>> assignRooms(const vector<vector<int>>& intervals) {
    int n = intervals.size();
    // order = (start, request idx), sorted by start
    vector<pair<int, int>> order;
    for (int i = 0; i < n; i++) { order.push_back({intervals[i][0], i}); }
    sort(order.begin(), order.end());
    // cars = (end of its last request, car id) per car, earliest end on top
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

// (3) peak concurrency ranges
// core idea: sweep events grouped by time; cnt after time t holds until next event time
// keep ranges where cnt == max, merge adjacent ones
// T: O(n log n), S: O(n)
vector<pair<int, int>> peakRanges(const vector<vector<int>>& intervals) {
    // events = (time, +1 start / -1 end); ends first at equal times
    vector<pair<int, int>> events;
    for (const auto& meeting : intervals) {
        events.push_back({meeting[0], 1});
        events.push_back({meeting[1], -1});
    }
    sort(events.begin(), events.end());
    int numEvents = events.size();
    int inUse = 0;
    int best = 0;
    // ranges = [from, to) where inUse == best
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

// (4) smallest free chair (LC 1942)
// core idea: go by arrival; move chairs of friends who left into a min-heap of free ids
// seat on the smallest free id, else on a new chair
// T: O(n log n), S: O(n)
int smallestChair(const vector<vector<int>>& times, int targetFriend) {
    int n = times.size();
    // order = (arrival, friend), sorted by arrival
    vector<pair<int, int>> order;
    for (int i = 0; i < n; i++) { order.push_back({times[i][0], i}); }
    sort(order.begin(), order.end());
    // freeChairs = ids of chairs freed so far, smallest on top
    priority_queue<int, vector<int>, greater<int>> freeChairs;
    // taken = (leaving time, chair id) of seated friends, earliest leave on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> taken;
    // next never-used chair id
    int nextChair = 0;
    for (const auto& [arrival, friendId] : order) {
        // <=: chair freed at t is free for an arrival at t
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

// follow-up:
// sweep line for (1) -> +1 at start, -1 at end, ends first at ties; max running sum
// can one person attend all (LC 252) -> sort; any start < prev end is a clash
// closed intervals / min groups (LC 2406) -> same as rooms, pop only ends < start
// 24-hour circle -> split a wrapping meeting into [start, 1440) and [0, end), then cnt
// min chairs / min CPUs -> same cnt as (1)
// combined schedule, roster merge shifts -> 09
// fixed num of rooms w/ delays -> meeting rooms III (08)
