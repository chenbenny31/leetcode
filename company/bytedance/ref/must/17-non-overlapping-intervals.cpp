// ByteDance (FastPrep, phone / onsite, 2 sightings) / LC 435: return the min number of
// intervals to remove so the rest are pairwise non-overlapping; touching at an endpoint
// is fine. Be ready to prove the greedy (HM asked): see below.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Greedy: sort by end, keep each interval starting at or after the last kept end.
// Proof (exchange): let g have the smallest end, o the smallest-end interval of an
// optimal set. end(g) <= end(o), so swapping o for g overlaps nothing new: some optimal
// set keeps g. Drop g and all it overlaps, repeat. T: O(n log n), S: O(1) besides sort.
int minimumIntervalRemovals(vector<vector<int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int n = intervals.size();
    if (n == 0) {
        return 0;
    }
    // the earliest-ending interval is always kept
    int kept = 1;
    // end of the last kept interval
    int lastEnd = intervals[0][1];
    for (int i = 1; i < n; i++) {
        // touching (start == lastEnd) is not an overlap
        if (intervals[i][0] >= lastEnd) {
            kept++;
            lastEnd = intervals[i][1];
        }
    }
    return n - kept;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    cout << label << (minimumIntervalRemovals(intervals) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("touching", {{1, 3}, {3, 5}, {5, 7}}, 0);
    test("nested", {{1, 10}, {2, 3}, {4, 5}}, 1);
    test("dups", {{0, 2}, {0, 2}}, 1);
    test("negative", {{-3, -1}, {-2, 0}, {-1, 1}}, 1);
    test("empty", {}, 0);
}

// follow-up: return the kept intervals -> push intervals[i] whenever kept++
// follow-up: min arrows (LC 452) -> same sort by end; touching counts as overlap, so a
//   new arrow only when start > lastEnd
// follow-up: weighted intervals (LC 1235) -> sort by end, dp[i] = max(dp[i-1], w_i +
//   dp[j]), j = last end <= start_i via binary search, O(n log n)
// follow-up: sort by start instead -> on overlap removed++, lastEnd = min(lastEnd, end)
