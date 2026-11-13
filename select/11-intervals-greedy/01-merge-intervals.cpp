// LC 56 Merge Intervals (Medium) — sort by start, extend the last merged interval
// merge all overlapping closed intervals [start, end] of an unsorted list
// return disjoint result in start order; touching [1,2] + [2,3] merge too
//
// clarify:
// closed or half-open -> closed, touching merges
// input sorted? -> no, sort first
// start <= end -> assume yes
// dup / nested intervals -> max on end handles both
// empty input -> {}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;

// core idea: after sort by start, next interval can only overlap the last merged one
// T: O(n log n), S: O(n)
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

// follow-up:
// half-open [s, e) -> touching stays apart: compare w/ < instead of <=
// insert into sorted disjoint list (LC 57) -> copy before, absorb overlaps, copy rest
// employee free time (LC 759) -> flatten + merge; gaps between merged blocks = answer
// online inserts, query merged set (LC 715) -> ordered map start -> end; absorb overlaps
// earliest common slot of given len (LC 1229) -> two pointers; [max start, min end]
// intersect two sorted lists (LC 986) -> 04-interval-list-intersections
// fewest removals to make disjoint (LC 435) -> 03-non-overlapping-intervals
// min interval to include each query (LC 1851) -> offline sort; min-heap by size
