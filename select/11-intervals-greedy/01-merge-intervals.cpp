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

// follow-up: insert into sorted disjoint intervals (LC 57) -> copy those ending before
//   new.start, absorb overlaps (min start / max end), copy the rest; O(n), no sort
// follow-up: open intervals [s, e) -> use cur[0] < merged.back()[1]
