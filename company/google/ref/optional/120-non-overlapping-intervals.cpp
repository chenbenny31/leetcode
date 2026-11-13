// 120 · Non-overlapping Intervals — LC 435, 452
// Mother: select/11-intervals-greedy/03-non-overlapping-intervals.cpp
// return min num of intervals to remove so the rest don't overlap
//
// clarify:
// touching endpoints ([1,3], [3,5]) -> not an overlap
// dups, negative values -> allowed
// empty input -> 0

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// core idea: sort by end; keep every interval w/ start >= last kept end
// earliest end leaves the most room; answer = n - kept
// T: O(n log n), S: O(1) — besides the sort
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

// follow-up:
// why greedy is optimal -> exchange: swap first kept interval for earliest-ending one
// min arrows (LC 452) -> same sort; touching = overlap, new arrow only if start > lastEnd
