// LC 57: intervals are sorted by start and non-overlapping; insert newInterval and merge
// so the result is still sorted and non-overlapping. Closed intervals: [1,2] and [2,3]
// overlap.
// PointOne angle: adding or editing one time entry in a sorted timeline.
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

// straightforward: append, sort, then the usual merge sweep. T: O(n log n), S: O(n).
vector<vector<int>> insert(vector<vector<int>> intervals,
                           const vector<int>& newInterval) {
    intervals.push_back(newInterval);
    sort(intervals.begin(), intervals.end());
    vector<vector<int>> merged;   // disjoint intervals in start order
    for (const auto& cur : intervals) {
        if (!merged.empty() && cur[0] <= merged.back()[1]) {
            merged.back()[1] = max(merged.back()[1], cur[1]);
        } else {
            merged.push_back(cur);
        }
    }
    return merged;
}

// improved: input is already sorted, so copy the part before, absorb the overlaps, copy
// the part after. T: O(n), S: O(n) output.
vector<vector<int>> insertLinear(const vector<vector<int>>& intervals,
                                 vector<int> newInterval) {
    vector<vector<int>> result;   // disjoint intervals in start order
    int n = intervals.size();
    int i = 0;
    // entirely before newInterval
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i]);
        i++;
    }
    // overlapping: grow newInterval to cover each one
    while (i < n && intervals[i][0] <= newInterval[1]) {
        newInterval[0] = min(newInterval[0], intervals[i][0]);
        newInterval[1] = max(newInterval[1], intervals[i][1]);
        i++;
    }
    result.push_back(newInterval);
    // entirely after
    while (i < n) {
        result.push_back(intervals[i]);
        i++;
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<int>& newInterval, const vector<vector<int>>& want) {
    bool ok = insert(intervals, newInterval) == want &&
              insertLinear(intervals, newInterval) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gap", {{1, 2}, {6, 7}}, {3, 4}, {{1, 2}, {3, 4}, {6, 7}});
    test("overlap", {{1, 3}, {4, 6}, {8, 9}}, {2, 5}, {{1, 6}, {8, 9}});
    test("touch", {{1, 2}}, {2, 3}, {{1, 3}});
    test("empty", {}, {1, 2}, {{1, 2}});
}
