// end open or close -> if close: <=
// empty input -> 0 rooms
// arrival times are distinct

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

// sort by start, min-heap of end time in running, pop if end <= placing start
// max heap size is answer, T: O(nlogn), S: O(n)
int minMeetingRooms(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    priority_queue<int, vector<int>, greater<int>> heap; // earliest end on top
    int result = 0;
    for (const auto& intv : intervals) {
        while (!heap.empty() && heap.top() <= intv[0]) {
            heap.pop();
        }
        heap.push(intv[1]);
        result = max(result, (int)heap.size());
    }
    return result;
}

// sort by start, re-use the car that frees first if avail with heap
// else create new car with id, reused car keep its id, T: O(nlogn), S: O(n)
pair<int, vector<int>> assignCars(const vector<vector<int>>& requests) {
    int n = requests.size();
    vector<pair<int, int>> order; // (start, req)
    for (int i = 0; i < n; i++) {
        order.push_back({requests[i][0], i});
    }
    sort(order.begin(), order.end());
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    vector<int> cars(n, -1); // car[req] = car-id
    int count = 0; // max num and next id of car

    for (const auto& [start, req] : order) {
        int id;
        if (!heap.empty() && heap.top().first <= start) { // have avail re-used car
            id = heap.top().second;
            heap.pop();
        } else { // need new car
            id = count;
            count++;
        }

        cars[req] = id;
        heap.push({requests[req][1], id});
    }
    return {count, cars};
}


// sweep-line, collect peak groups, merge touching ones, T: O(nlogn), S: O(n)
vector<pair<int, int>> peakRanges(const vector<vector<int>>& intervals) {
    vector<pair<int, int>> events; // (time, +1 start / -1 end)
    for (const auto& intv : intervals) {
        events.push_back({intv[0], +1});
        events.push_back({intv[1], -1});
    }
    sort(events.begin(), events.end());
    int n = events.size();
    int run = 0; // num of rooms running
    int peak = 0; // max num of rooms running

    vector<pair<int, int>> result;
    int i = 0;
    while (i < n) {
        int t = events[i].first;
        while (i < n && events[i].first == t) {
            run += events[i].second;
            i++;
        }

        if (i == n) { break; }
        int nt = events[i].first; // nt > t
        if (run > peak) { // found a bigger peak
            peak = run;
            result.clear();
        }
        if (run == 0 || run < peak) { continue; }

        if (!result.empty() && result.back().second == t) {
            result.back().second = nt;
        } else {
            result.push_back({t, nt});
        }
    }
    return result;
}

// taken chair top by min end time, freed chair top by min avail time, else new
// T: O(nlogn), S: O(n)
int smallestChair(const vector<vector<int>>& times, int targetFriend) {
    int n = times.size();
    vector<pair<int, int>> order; // (arrival, friend), min arrival first
    for (int i = 0; i < n; i++) {
        order.push_back({times[i][0], i});
    }
    sort(order.begin(), order.end());
    // (end-time, chair-id), min end time on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> taken;
    // chair-id, min on top
    priority_queue<int, vector<int>, greater<int>> freed;
    int count = 0;
    for (const auto& [arrival, i] : order) {
        while (!taken.empty() && taken.top().first <= arrival) {
            freed.push(taken.top().second);
            taken.pop();
        }

        int id;
        if (!freed.empty()) {
            id = freed.top();
            freed.pop();
        } else {
            id = count;
            count++;
        }

        if (i == targetFriend) { return id; }
        taken.push({times[i][1], id});
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minMeetingRooms(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testAssign(const string& label, const vector<vector<int>>& requests, int wantCount,
                const vector<int>& wantCars) {
    auto [count, cars] = assignCars(requests);
    bool ok = count == wantCount && cars == wantCars;
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
