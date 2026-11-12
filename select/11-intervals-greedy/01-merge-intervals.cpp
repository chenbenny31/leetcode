// LC 56 Merge Intervals (Medium). Pattern: sort by start, extend the last interval.
// Merge all overlapping closed intervals [start, end]; touching ones like [1,2] and
// [2,3] merge too. Input is not sorted.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;

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
    cout << label << (employeeFreeTime(schedules) == want ? " PASS\n" : " FAIL\n");
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
}

// follow-up: open intervals [s, e) -> use cur[0] < merged.back()[1]
