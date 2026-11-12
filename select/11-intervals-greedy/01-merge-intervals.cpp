// LC 56 Merge Intervals (Medium). Pattern: sort by start, extend the last interval.
// Merge all overlapping closed intervals [start, end]; touching ones like [1,2] and
// [2,3] merge too. Input is not sorted.
//
// Clarify:
// - closed or half-open bounds? -> closed: [1,2] and [2,3] touch and merge; with [s, e)
//   they stay apart (follow-up)
// - sorted input? start <= end in each? -> unsorted, start <= end; sort a copy
// - duplicates, one interval inside another -> allowed; each group collapses into one
// - empty input -> empty output; output order -> by start
// - n up to ~1e4 -> sort, O(n log n); values fit in int
// Idea: after sorting by start, the next interval can only overlap the last merged one
//   (every earlier merged block ends before the last one starts). One pass either
//   extends the last merged interval or opens a new one.
// Pitfalls:
// - max when extending: a contained interval ([1,10] then [2,3]) must not cut the end
// - compare with merged.back(), not the previous input interval ([1,10], [2,3], [4,5])
// - <= for closed bounds; sort a copy (by value), not the caller's vector
// Tests to write: unsorted overlap; touching -> one; contained -> outer kept; empty.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;
using std::priority_queue;
using std::tuple;
using std::get;
using std::greater;

// Sort by start, then either extend the last merged interval or start a new one.
// T: O(n log n), S: O(n).
vector<vector<int>> merge(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // disjoint intervals in start order
    vector<vector<int>> merged;
    for (const auto& cur : intervals) {
        // closed intervals: start == last end still overlaps
        if (!merged.empty() && cur[0] <= merged.back()[1]) {
            // max: cur may sit entirely inside the last one
            merged.back()[1] = max(merged.back()[1], cur[1]);
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

// Pitfall: push newInterval after the absorb loop even when nothing overlaps or it goes
// last; the first loop's strict < leaves an interval ending at newInterval[0] to absorb.
// LC 57: intervals are sorted and disjoint; copy those ending before newInterval,
// absorb every overlap into it (min start / max end), copy the rest. T: O(n), S: O(n).
vector<vector<int>> insertInterval(const vector<vector<int>>& intervals,
                                   vector<int> newInterval) {
    // intervals in start order with newInterval merged in
    vector<vector<int>> result;
    int n = intervals.size();
    int i = 0;
    // closed intervals: ending exactly at the new start still overlaps
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i]);
        i++;
    }
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);
    while (i < n) {
        result.push_back(intervals[i]);
        i++;
    }
    return result;
}

// LC 759: flatten every employee's intervals, merge them as above (touching ones merge),
// and the free times are the gaps between consecutive merged intervals.
// T: O(n log n), S: O(n); n = total intervals.
vector<vector<int>> employeeFreeTime(const vector<vector<vector<int>>>& schedules) {
    // every working interval of every employee
    vector<vector<int>> all;
    for (const auto& schedule : schedules) {
        for (const auto& interval : schedule) { all.push_back(interval); }
    }
    // disjoint busy blocks in start order
    vector<vector<int>> busy = merge(all);
    vector<vector<int>> result;
    for (int i = 1; i < (int)busy.size(); i++) {
        result.push_back({busy[i - 1][1], busy[i][0]});
    }
    return result;
}

// follow-up (LC 759 optimal): each schedule is already sorted, so k-way merge them with a
// min-heap of each employee's next interval instead of sorting all n; a start after the
// latest end seen so far opens a gap. T: O(n log k), S: O(k); k = employees.
vector<vector<int>> employeeFreeTimeHeap(const vector<vector<vector<int>>>& schedules) {
    int k = schedules.size();
    // (start, employee, index in that schedule) of each employee's next interval
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>> heap;
    for (int emp = 0; emp < k; emp++) {
        // an employee may have no intervals
        if (!schedules[emp].empty()) { heap.push({schedules[emp][0][0], emp, 0}); }
    }
    if (heap.empty()) { return {}; }
    // latest end among the intervals popped so far; starts at the earliest start
    int busyUntil = get<0>(heap.top());
    vector<vector<int>> result;
    while (!heap.empty()) {
        auto [start, emp, idx] = heap.top();
        heap.pop();
        // closed intervals: start == busyUntil touches, no gap
        if (start > busyUntil) { result.push_back({busyUntil, start}); }
        busyUntil = max(busyUntil, schedules[emp][idx][1]);
        if (idx + 1 < (int)schedules[emp].size()) {
            heap.push({schedules[emp][idx + 1][0], emp, idx + 1});
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

void testInsert(const string& label, const vector<vector<int>>& intervals,
                const vector<int>& newInterval, const vector<vector<int>>& want) {
    bool ok = insertInterval(intervals, newInterval) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testFree(const string& label, const vector<vector<vector<int>>>& schedules,
              const vector<vector<int>>& want) {
    bool ok = employeeFreeTime(schedules) == want
              && employeeFreeTimeHeap(schedules) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});
    test("touching", {{1, 2}, {2, 3}}, {{1, 3}});
    test("contained", {{1, 10}, {2, 3}}, {{1, 10}});
    test("empty", {}, {});
    testInsert("insert-absorb-touching", {{1, 2}, {4, 5}, {7, 8}, {10, 12}}, {2, 7},
               {{1, 8}, {10, 12}});
    testInsert("insert-into-gap", {{1, 2}, {8, 9}}, {4, 5}, {{1, 2}, {4, 5}, {8, 9}});
    testInsert("insert-at-end", {{1, 2}}, {5, 6}, {{1, 2}, {5, 6}});
    testInsert("insert-into-empty", {}, {3, 4}, {{3, 4}});
    testFree("free-overlap-across", {{{1, 3}, {6, 7}}, {{2, 4}}}, {{4, 6}});
    testFree("free-touching", {{{1, 2}}, {{2, 3}}}, {});
    testFree("free-nested", {{{1, 10}}, {{2, 3}, {12, 13}}}, {{10, 12}});
    testFree("free-empty-schedule", {{}, {{1, 2}}, {{4, 5}}}, {{2, 4}});
    testFree("free-no-employees", {}, {});
}

// follow-up: open intervals [s, e) -> use cur[0] < merged.back()[1]
// follow-up: schedules already sorted (LC 759) -> k-way heap merge, employeeFreeTimeHeap
//   above
// follow-up: total length covered -> merge, then sum (long long if values are large):
//     long long total = 0;
//     for (const auto& block : merge(intervals)) { total += block[1] - block[0]; }
// follow-up: intervals arrive one at a time, merged set queried any time -> map start ->
//   end, merge on insert: RangeModule::addRange in
//   company/google/ref/recommended/21-interval-membership.cpp
// follow-up: intersect two sorted lists (LC 986) -> 04-interval-list-intersections.cpp
// follow-up: fewest removals to leave the rest disjoint (LC 435) ->
//   03-non-overlapping-intervals.cpp
