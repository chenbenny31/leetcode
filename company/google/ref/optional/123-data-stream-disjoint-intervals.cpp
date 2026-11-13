// 123 · Data Stream as Disjoint Intervals — LC 352
// addNum(value) adds a non-negative int (dups possible)
// getIntervals() = sorted disjoint [start, end] covering all values, adjacent merged
//
// clarify:
// re-add a covered value -> no-op
// adjacent values -> merged ([1,2] + 3 -> [1,3])
// ratio of adds to queries -> many queries: keep the map merged on every add

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// core idea: ordered map start -> end; look at the interval before and after value
// covered -> nothing; else extend left, absorb right, both, or insert [value, value]
// T: addNum O(log k), getIntervals O(k), S: O(k) — k = num intervals
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
    ranges.addNum(4);
    ranges.addNum(7);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 4}, {7, 7}}, "extend-left");
    ranges.addNum(6);
    check(ranges.getIntervals() == vector<vector<int>>{{1, 4}, {6, 7}}, "join-right");
}

// follow-up:
// Range Module (optional 42) -> same map of disjoint intervals w/ range add / remove
