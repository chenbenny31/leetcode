// Given closed intervals [start, end] (both ends included), return the max number of
// intervals that share a single point. (Variation of LC 253.)

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
using std::max;

// straightforward: the max is reached at some interval's start, so count the intervals
// covering each start. T: O(n^2), S: O(1).
int maxOverlap(const vector<vector<int>>& intervals) {
    int best = 0;
    for (const auto& a : intervals) {
        int cover = 0;
        for (const auto& b : intervals) {
            if (b[0] <= a[0] && a[0] <= b[1]) { cover++; }
        }
        best = max(best, cover);
    }
    return best;
}

// improved: sweep sorted events; at the same point a start (0) comes before an end (1)
// because closed intervals touching at x both cover x. T: O(n log n), S: O(n).
int maxOverlapSweep(const vector<vector<int>>& intervals) {
    // events = (point, 0 for start / 1 for end)
    vector<pair<int, int>> events;
    for (const auto& iv : intervals) {
        events.push_back({iv[0], 0});
        events.push_back({iv[1], 1});
    }
    sort(events.begin(), events.end());
    int open = 0, best = 0;
    for (const auto& [x, type] : events) {
        if (type == 0) {
            open++;
            best = max(best, open);
        } else {
            open--;
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& intervals, int want) {
    bool ok = maxOverlap(intervals) == want && maxOverlapSweep(intervals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("touch", {{1, 2}, {2, 3}}, 2);
    test("disjoint", {{1, 1}, {3, 4}}, 1);
    test("nested", {{1, 10}, {2, 3}, {2, 2}}, 3);
    test("negative", {{-5, -1}, {-3, 0}, {1, 2}}, 2);
    test("single", {{0, 0}}, 1);
}
