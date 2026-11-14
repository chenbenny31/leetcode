// end exclusive or inclusive
// empty input
// distinct arrival time?

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

// sort by start, min-heap of end times running, pop when curStart >= top-end, max(heap-size) is result
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> end; // min-heap, top by min end time, running rooms
    int rooms = 0;
    for (const auto& m : intervals) {
        while (!ends.empty() && ends.top() <= m[0]) { // pop rooms un-used by now
            ends.pop();
        }
        ends.push(m[1]);
        rooms = max(rooms, (int)ends.size());
    }
    return rooms;
}

// re-use car that frees first with heap, else use new car, reused car keep id
pair<int, vector<int>> assignRooms(const vector<vector<int>>& intervals) {
    int n = intervals.size();
    vector<pair<int, int>> order; // (start, req) sorted by start
    for (int i = 0; i < n; i++) {
        order.push_back({intervals[i][0], i});
    }
    sort(order.begin(), order.end());
    // (avail-time, car-id), top by min of avail-time
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;

    vector<int> cars(n, -1); // car[req] = car-id
    int count = 0;
    for (const auto& [start, req] : order) {
        int id;
        if (!heap.empty() && heap.top().first <= start) {
            id = heap.top().second;
            heap.pop();
        } else { // every car is busy
            id = count;
            count++;
        }
        cars[req] = id;
        heap.push({intervals[req][1], id});
    }
    return {count, cars};
}

// sweep-line track max peak concurrency, apply event with time-i, collect intvs
vector<pair<int, int>> peakRanges(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events; // (time, +1 for start / -1 for end)
    for (const auto& intv : intervals) {
        events.push_back({intv[0], +1});
        events.push_back({intv[1], -1});
    }
    sort(events.begin(), events.end());
    int n = events.size();
    int running = 0;
    int peak = 0;
    vector<pair<int, int>> result; // [from, to) ranges if running = peak
    int i = 0;

    while (i < n) {
        int t = events[i].first;
        while (i < n && events[i].first == t) {
            running += events[i].second;
            i++;
        }

        if (i == n) { break; }

        int nt = events[i].first;
        if (running > peak) { // running holds on [t, nt)
            peek = running;
            result.clear();
        }
        if (running == 0 || running < peak) { continue; }

        if (!result.empty() && result.back().second == t) { // merge with larger end
            result.back().second = nt;
        } else { // add a new one
            result.push_back({t, nt});
        }
    }
    return result;
}

// two min-heap taken and freed tracking used and freed chairs
int smallestChair(const vector<vector<int>>& time, int targetFriend) {
    int n = times.size();
    vector<pair<int, int>> order; // (arrival, friend), smaller arrival first
    for (int i = 0; i < n; i++) {
        order.push_back({time[i][0], i});
    }
    sort(order.begin(), order.end());
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> taken; // min-heap top by min time
    priority_queue<int, vector<int>, greater<int>> freed; // chairs freed, min on top

    int count;
    for (const auto& [t, i] : order) {
        while (!taken.empty() && taken.top().first <= t) {
            freed.push(taken.top().second);
            taken.pop();
        }

        int chair;
        if (!freed.empty()) {
            chair = freed.top();
            freed.top();
        } else {
            chair = count;
            count++;
        }

        if (i == targetFriend) { return chair; }
        taken.push({time[i][1], chair});
    }
    return -1;
}
