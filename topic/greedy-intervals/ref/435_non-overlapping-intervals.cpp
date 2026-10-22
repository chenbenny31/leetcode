// LC 435 Non-overlapping Intervals: return the minimum number of intervals to remove
// so the rest do not overlap. Intervals that only touch ([1,2], [2,3]) do not overlap.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort by end and keep every interval that starts at or after the last kept end: the
// earliest end leaves the most room (exchange argument). T: O(n log n), S: O(1).
int eraseOverlapIntervals(vector<vector<int>> intervals) {
    int n = intervals.size();
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int kept = 0;
    int lastEnd = INT_MIN;   // end of the last kept interval
    for (const auto& iv : intervals) {
        if (iv[0] >= lastEnd) {
            kept++;
            lastEnd = iv[1];
        }
    }
    return n - kept;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (eraseOverlapIntervals(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("drop-long", {{1, 5}, {2, 3}, {4, 6}}, 1);
    test("touching-ok", {{1, 2}, {2, 3}}, 0);
    test("duplicates", {{1, 2}, {1, 2}, {1, 2}}, 2);
    test("empty", {}, 0);
}
