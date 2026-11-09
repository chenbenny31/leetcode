// PlusAI (JD): Coverage Gaps (Medium). Pattern: sort + merge intervals.
// Uploads of one drive delivered chunks [start, end), unsorted and possibly overlapping.
// Given the drive's window [t0, t1), return the missing sub-intervals [a, b) in time
// order (chunks that only touch, like [0, 5) and [5, 9), leave no gap).

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::sort;
using std::min;
using std::max;

// This is LC 56 Merge Intervals: sort by start, `covered` is the end of the current
// merged run, and a chunk starting past it leaves a gap. T: O(n log n), S: O(n).
vector<pair<int, int>> coverageGaps(vector<pair<int, int>> chunks, int t0, int t1) {
    sort(chunks.begin(), chunks.end());
    vector<pair<int, int>> gaps;
    // [t0, covered) is fully uploaded so far
    int covered = t0;
    for (auto [start, end] : chunks) {
        if (covered >= t1) { break; }
        if (start > covered) {
            // clip at t1: a chunk may start after the window ends
            gaps.push_back({covered, min(start, t1)});
        }
        covered = max(covered, end);
    }
    if (covered < t1) { gaps.push_back({covered, t1}); }
    return gaps;
}

void test(const string& label, const vector<pair<int, int>>& chunks, int t0, int t1,
          const vector<pair<int, int>>& want) {
    cout << label << (coverageGaps(chunks, t0, t1) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap-unsorted", {{5, 7}, {0, 3}, {2, 4}}, 0, 10, {{4, 5}, {7, 10}});
    test("touching-no-gap", {{5, 10}, {0, 5}}, 0, 10, {});
    test("chunks-past-window", {{0, 12}, {18, 30}}, 10, 20, {{12, 18}});
    test("no-chunks", {}, 0, 10, {{0, 10}});
}

// follow-up: chunks keep arriving and gaps are queried any time -> keep a map<start, end>
//   of merged intervals, merging neighbors on insert (LC 715 Range Module); a query
//   walks the map from upper_bound(t0) minus one
