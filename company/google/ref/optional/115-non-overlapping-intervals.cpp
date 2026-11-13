// 115 · Non-overlapping Intervals — LC 435, 452
// Mother problem: select/11-intervals-greedy/03-non-overlapping-intervals.cpp
//
// Clarify
// - touching endpoints? → [1,3] and [3,5] do not overlap
// - return? → min number of intervals to remove
// - duplicates / negatives? → allowed
// - empty input? → 0
//
// Core idea
// - Sort by end; keep an interval when its start ≥ the last kept end, else drop it.
// - Answer = n − kept. T O(n log n), S O(1) besides the sort.
//
// Tests: touching, nested, dups, negative
//
// Follow-ups
// - why greedy is optimal → exchange: swap in the earliest-ending interval, repeat on
//   the rest
// - min arrows to burst balloons (LC 452) → same sort by end; touching counts as
//   overlap, so a new arrow only when start > lastEnd; answer = arrows (kept count)
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort by end; keep every interval that starts at or after the last kept end.
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
