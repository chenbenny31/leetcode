// LC 352 Data Stream as Disjoint Intervals: addNum(value) adds a non-negative int
// (duplicates possible); getIntervals() returns sorted disjoint [start, end] intervals
// covering every value added so far, with adjacent values merged ([1,2] + 3 -> [1,3]).
//
// Clarify:
// - re-adding a covered value is a no-op
// - ratio of adds to queries? -> map gives O(log k) add, O(k) query
// Idea: map ranges = start -> end of disjoint intervals. For addNum(v): next =
//   upper_bound(v), left = the interval before it. left covers v -> return.
//   joinLeft = left.end + 1 == v, joinRight = next.start == v + 1. Both -> extend left
//   to next.end and erase next; left only -> left.end = v; right only -> ranges[v] =
//   next.end, erase next; neither -> ranges[v] = v.
// Pitfalls:
// - check `next != begin` before stepping back to left
// - read next->second before erasing next
// Tests to write: two singles; merge both sides; duplicate; extend left; join right.

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// ordered map of disjoint intervals. T: O(log k) add, O(k) getIntervals, S: O(k).
class SummaryRanges {
    // start -> end of each disjoint interval
    map<int, int> ranges;

public:
    void addNum(int value) {
        // first interval starting after value
        auto next = ranges.upper_bound(value);
        bool joinRight = next != ranges.end() && next->first == value + 1;
        if (next != ranges.begin()) {
            auto left = next;
            left--;
            if (left->second >= value) { return; }
            if (left->second + 1 == value) {
                left->second = value;
                if (joinRight) {
                    left->second = next->second;
                    ranges.erase(next);
                }
                return;
            }
        }
        int end = value;
        if (joinRight) {
            end = next->second;
            ranges.erase(next);
        }
        ranges[value] = end;
    }

    vector<vector<int>> getIntervals() {
        vector<vector<int>> result;
        for (auto [start, end] : ranges) { result.push_back({start, end}); }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SummaryRanges ranges;
    ranges.addNum(1);
    ranges.addNum(3);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 1}, {3, 3}}, "singles");
    ranges.addNum(2);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 3}}, "merge-both");
    ranges.addNum(2);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 3}}, "duplicate");
    ranges.addNum(4);
    ranges.addNum(7);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 4}, {7, 7}}, "extend-left");
    ranges.addNum(6);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 4}, {6, 7}}, "join-right");
}

// follow-up: Range Module (optional 42) -> same map of disjoint intervals with range
//   add / remove
// follow-up: many duplicates, few intervals -> same; the covered check returns early
