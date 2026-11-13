// 39 · Merge Intervals — LC 56, 57, 759, 1229
// Mother problem: select/11-intervals-greedy/01-merge-intervals.cpp
//
// Google reports: employee free time, LC 56 ×2, roster merge shifts (09)
//
// LC 56: merge all overlapping closed intervals [start, end]; touching ones like [1,2]
// and [2,3] merge too. Input is not sorted.
//
// Clarify
// - closed bounds → [1,2] and [2,3] merge; unsorted, start <= end; sort a copy
// - duplicates / nesting allowed; empty → empty; output by start
//
// Core idea
// - After sorting by start, the next interval can only overlap the last merged one.
// - Extend with max (a contained interval must not cut the end) or open a new one.
// - Compare with merged.back(), not the previous input interval ([1,10], [2,3], [4,5]).
// - T O(n log n), S O(n).
//
// Tests
// - unsorted overlap; touching → one; contained → outer kept; empty
//
// Follow-ups
// - insert into a sorted disjoint list (LC 57) → copy those ending before, absorb every
//   overlap (min start / max end), push it, copy the rest, O(n)
// - employee free time (LC 759) → flatten, merge, the gaps between merged blocks;
//   schedules already sorted → k-way min-heap of each employee's next interval,
//   O(n log k)
// - meeting scheduler (LC 1229) → sort both, overlap = [max start, min end], advance the
//   slot that ends first
// - half-open [s, e) → compare with <; total covered length → merge, then sum
// - streaming inserts → interval membership (42); LC 986 → interval list intersections
//   (93); LC 435 → non-overlapping intervals (115)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;

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

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<vector<int>>& want) {
    cout << label << (merge(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unsorted", {{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}});
    test("touching", {{1, 2}, {2, 3}}, {{1, 3}});
    test("contained", {{1, 10}, {2, 3}}, {{1, 10}});
    test("empty", {}, {});
}
