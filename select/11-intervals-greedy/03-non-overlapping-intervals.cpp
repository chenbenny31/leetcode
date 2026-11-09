// LC 435 Non-overlapping Intervals (Medium). Pattern: greedy, sort by end, keep earliest.
// Return the min number of intervals to remove so the rest are pairwise
// non-overlapping; touching at an endpoint is fine.

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

// follow-up: why the greedy is optimal -> exchange: the earliest-ending interval can
//   replace the first one of any optimal set without new overlaps; repeat on the rest
// follow-up: min arrows (LC 452) -> same sort; touching counts as overlap, so a new
//   arrow only when start > lastEnd
