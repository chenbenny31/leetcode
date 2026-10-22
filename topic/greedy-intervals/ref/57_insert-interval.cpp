// LC 57 Insert Interval: intervals are disjoint and sorted by start. Insert
// newInterval, merging any overlaps (touching counts), and keep the result sorted.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Three phases: copy those ending before, absorb overlaps, copy the rest.
// T: O(n), S: O(n).
vector<vector<int>> insert(const vector<vector<int>>& intervals,
                           vector<int> newInterval) {
    vector<vector<int>> result;   // disjoint intervals in start order
    int n = intervals.size();
    int i = 0;
    while (i < n && intervals[i][1] < newInterval[0]) {
        result.push_back(intervals[i]);
        i++;
    }
    // overlaps: grow newInterval to cover each one
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

void test(const string& label, const vector<vector<int>>& intervals,
          const vector<int>& newInterval, const vector<vector<int>>& want) {
    cout << label << (insert(intervals, newInterval) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("merge-two", {{1, 3}, {6, 9}, {11, 12}}, {2, 7}, {{1, 9}, {11, 12}});
    test("gap-between", {{1, 2}, {8, 9}}, {4, 5}, {{1, 2}, {4, 5}, {8, 9}});
    test("touching-end", {{1, 2}}, {2, 3}, {{1, 3}});
    test("empty", {}, {5, 7}, {{5, 7}});
}
