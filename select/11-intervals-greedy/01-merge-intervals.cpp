// LC 56 Merge Intervals (Medium) — sort by start, extend the last merged interval
// Merge all overlapping closed intervals [start, end] of an unsorted list and return
// the disjoint result in start order; touching ones like [1,2] and [2,3] merge too.
// Clarify: closed or half-open bounds; input sorted or not; start <= end; duplicates
//   and nested intervals; empty input; may the input be modified.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;

// Core idea: after sorting by start, the next interval can only overlap the last merged
// one, so one pass either extends it or opens a new one. T: O(n log n), S: O(n).
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

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});
    test("touching", {{1, 2}, {2, 3}}, {{1, 3}});
    test("contained", {{1, 10}, {2, 3}, {4, 5}}, {{1, 10}});
    test("empty", {}, {});
    return 0;
}

// follow-up (ideas only):
// - half-open intervals [s, e) -> touching ones stay apart: compare with < instead of <=
// - insert into a sorted disjoint list (LC 57) -> copy those ending before the new start,
//   absorb every overlap into the new one (min start / max end), push it, copy the rest
// - employee free time (LC 759) -> flatten and merge; the gaps between consecutive merged
//   blocks are the answer. Sorted schedules: k-way heap merge tracking the latest end
// - total length covered -> merge, then sum end - start in long long
// - intervals arrive one at a time, merged set queried any time (LC 715) -> ordered map
//   start -> end; on insert absorb every overlapping entry, then store the union
// - earliest common slot of a given length (LC 1229) -> sort both lists, two pointers:
//   overlap = [max start, min end]; advance the slot that ends first
// - intersect two sorted lists (LC 986) -> 04-interval-list-intersections
// - fewest removals to leave the rest disjoint (LC 435) -> 03-non-overlapping-intervals
// - minimum interval to include each query (LC 1851) -> sort intervals and queries
//   (offline); min-heap by size: push those with start <= q, pop those with end < q
