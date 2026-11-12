// Non-Overlapping Interval Range Membership Data Structure (Google, interview report)
// add(start, end) stores the inclusive integer interval [start, end] (start <= end); it
// never overlaps a stored interval, but intervals may arrive in any order.
// contains(x) returns whether x lies in some stored interval.
// Part 2, LC 715 Range Module: half-open ranges [left, right) that may overlap.
// addRange covers [left, right), removeRange uncovers it, queryRange(left, right) says
// whether every number in [left, right) is covered.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::min;
using std::max;
using std::prev;

// straightforward: keep a list of intervals and scan all of them.
// T: add O(1), contains O(N), S: O(N).
class IntervalList {
    // stored {start, end} pairs in insertion order
    vector<pair<int, int>> intervals;

public:
    void add(int start, int end) { intervals.push_back({start, end}); }

    bool contains(int x) {
        for (auto [start, end] : intervals) {
            if (start <= x && x <= end) { return true; }
        }
        return false;
    }
};

// improved: intervals sorted by start; since they are disjoint, the only candidate for x
// is the last interval starting at or before x. T: add O(log N), contains O(log N),
// S: O(N).
class IntervalMap {
    // start -> end of each stored interval, ascending by start
    map<int, int> endOf;

public:
    void add(int start, int end) { endOf[start] = end; }

    bool contains(int x) {
        // first interval starting after x
        auto it = endOf.upper_bound(x);
        if (it == endOf.begin()) { return false; }
        it--;
        return x <= it->second;
    }
};

// part 2: same sorted map, but kept disjoint and non-touching by merging on add and
// splitting on remove. T: add / remove O(log N) amortized (each range is erased at most
// once after it is created), query O(log N). S: O(N).
class RangeModule {
    // start -> end of each covered half-open range; disjoint, no two touching
    map<int, int> ranges;

public:
    void addRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        // the range before left also merges if it reaches left
        if (it != ranges.begin() && prev(it)->second >= left) { it--; }
        // absorb every range that overlaps or touches [left, right)
        while (it != ranges.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            it = ranges.erase(it);
        }
        ranges[left] = right;
    }

    void removeRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second > left) { it--; }
        // cut every range overlapping [left, right), keeping its parts outside
        while (it != ranges.end() && it->first < right) {
            int start = it->first;
            int end = it->second;
            it = ranges.erase(it);
            if (start < left) { ranges[start] = left; }
            if (end > right) { ranges[right] = end; }
        }
    }

    // only the range containing left can cover [left, right), since ranges never touch
    bool queryRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it == ranges.begin()) { return false; }
        it--;
        return right <= it->second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void test(const string& label, const vector<pair<int, int>>& intervals,
          const vector<int>& queries, const vector<bool>& want) {
    IntervalList list;
    IntervalMap sorted;
    for (auto [start, end] : intervals) {
        list.add(start, end);
        sorted.add(start, end);
    }
    bool ok = true;
    for (int q = 0; q < (int)queries.size(); q++) {
        ok = ok && list.contains(queries[q]) == want[q];
        ok = ok && sorted.contains(queries[q]) == want[q];
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("inside-gap", {{1, 3}, {6, 8}}, {2, 4, 7}, {true, false, true});
    test("endpoints", {{1, 3}}, {0, 1, 3, 4}, {false, true, true, false});
    test("point", {{5, 5}}, {4, 5, 6}, {false, true, false});
    test("any-order", {{6, 8}, {1, 3}}, {0, 2, 5, 9}, {false, true, false, false});
    test("empty", {}, {0}, {false});

    RangeModule module;
    module.addRange(10, 20);
    module.addRange(20, 30);
    check(module.queryRange(10, 30) && !module.queryRange(10, 31), "touch-merge");
    module.removeRange(14, 16);
    check(module.queryRange(10, 14) && module.queryRange(16, 30)
          && !module.queryRange(10, 15), "split");
    module.addRange(12, 18);
    check(module.queryRange(10, 30), "bridge");
    module.addRange(40, 50);
    module.addRange(5, 45);
    check(module.queryRange(5, 50) && !module.queryRange(4, 6), "absorb-many");
    module.removeRange(0, 100);
    check(!module.queryRange(10, 11), "remove-all");
}

// follow-up: O(1) contains -> bounded domain [0, D): a bitset with one bit per value;
//   add sets bits start..end (O(length)), contains tests bit x (O(1)).
//   Memory is D / 8 bytes no matter how few intervals: D = 10^9 -> 125 MB,
//   all 32-bit ints -> 512 MB. A hash set of covered values costs ~30+ bytes per value.
//     vector<bool> covered(D);  // covered[v] = some interval contains v
// follow-up: overlapping adds allowed -> on add, erase every map entry overlapping
//   [start, end] and insert the merged interval; each interval is erased at most once
//   (RangeModule::addRange above, half-open)
