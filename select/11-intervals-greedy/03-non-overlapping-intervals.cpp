// LC 435 Non-overlapping Intervals (Medium) — greedy, sort by end, keep earliest end
// Return the min number of intervals to remove so the rest are pairwise
// non-overlapping; touching at an endpoint is not an overlap.
// Clarify: touching endpoints overlap or not; duplicate intervals; empty input;
//   negative coordinates.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Core idea: sort by end and keep every interval that starts at or after the last kept
// end; the earliest end leaves the most room for the rest. Answer = n - kept.
// T: O(n log n), S: O(1) besides the sort.
int eraseOverlapIntervals(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int n = intervals.size();
    int kept = 0;
    // end of the last kept interval; INT_MIN so the first interval is always kept
    int lastEnd = INT_MIN;
    for (const auto& cur : intervals) {
        // touching (start == lastEnd) is not an overlap
        if (cur[0] >= lastEnd) {
            kept++;
            lastEnd = cur[1];
        }
    }
    return n - kept;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (eraseOverlapIntervals(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("touching", {{1, 3}, {3, 5}, {5, 7}}, 0);
    test("nested", {{1, 10}, {2, 3}, {4, 5}}, 1);
    test("dups", {{0, 2}, {0, 2}}, 1);
    test("negative", {{-3, -1}, {-2, 0}, {-1, 1}}, 1);
}

// follow-up (ideas only):
// - why the greedy is optimal -> exchange argument: the earliest-ending interval can
//   replace the first one of any optimal set without new overlaps; repeat on the rest
// - min arrows to burst balloons (LC 452) -> same sort by end; touching counts as
//   overlap there, so a new arrow only when start > lastEnd
