// 43 · Merge Intervals — LC 56, 57, 759, 1229
// Mother: select/11-intervals-greedy/01-merge-intervals.cpp
// LC 56: merge all overlapping closed intervals; touching [1,2] [2,3] merge; unsorted
// reported: employee free time, LC 56 ×2, roster merge shifts (10)
//
// clarify:
// closed or half-open -> closed, touching ones merge
// sorted input? -> no, sort by start
// dups / nesting -> handled by max(end)
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

// core idea: sort by start; next interval can only overlap the last merged one
// T: O(n log n), S: O(n)
vector<vector<int>> merge(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end());
    // disjoint intervals in start order
    vector<vector<int>> merged;
    for (const auto& cur : intervals) {
        // closed: start == last end still overlaps
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

// follow-up:
// insert into sorted disjoint list (LC 57) -> copy before, absorb overlaps, copy rest
// employee free time (LC 759) -> flatten, merge, take gaps; k-way min-heap if sorted
// meeting scheduler (LC 1229) -> sort both, overlap = [max start, min end], advance min
// half-open [s, e) -> compare w/ < instead of <=
// total covered len -> merge, then sum lens
// streaming inserts -> ordered map of disjoint intervals (42-interval-membership)
