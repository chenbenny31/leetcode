// Non-Overlapping Interval Range Membership Data Structure (Google, interview report)
// (1) add(start, end) inclusive, never overlaps a stored one, any order; contains(x)
// (2) LC 715: half-open [left, right), may overlap; addRange, removeRange, queryRange
//
// clarify:
// inclusive ends -> one-point intervals ok
// adds that touch ([1, 3], [4, 6]) -> stay separate in (1)
// N, Q up to 1e5 -> need O(log N) per op
// value range -> small: bitset; up to 1e9: map

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

// (1) brute force
// core idea: keep a list of intervals, scan all of them
// T: add O(1), contains O(N), S: O(N)
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

// (1) sorted map
// core idea: disjoint intervals sorted by start are sorted by end too
// only candidate for x = last interval starting at or before x
// T: add O(log N), contains O(log N), S: O(N)
class IntervalMap {
    // start -> end of each stored interval, ascending by start
    map<int, int> endOf;

public:
    void add(int start, int end) { endOf[start] = end; }

    bool contains(int x) {
        // first interval starting after x; the one before it is the candidate
        auto it = endOf.upper_bound(x);
        if (it == endOf.begin()) { return false; }
        it--;
        return x <= it->second;
    }
};

// (2) range module
// core idea: same sorted map, kept disjoint and non-touching
// add merges every range it overlaps or touches; remove cuts them, keeps outside pieces
// T: add / remove O(log N) amortized, query O(log N), S: O(N)
class RangeModule {
    // start -> end of each covered half-open range; disjoint, no two touching
    map<int, int> ranges;

public:
    void addRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        // range before left also merges if it reaches left
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
        // cut every range overlapping [left, right), keep its parts outside
        while (it != ranges.end() && it->first < right) {
            int start = it->first;
            int end = it->second;
            it = ranges.erase(it);
            if (start < left) { ranges[start] = left; }
            if (end > right) { ranges[right] = end; }
        }
    }

    // only the range containing left can cover [left, right): ranges never touch
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
}

// follow-up:
// O(1) contains -> bounded domain [0, D): one bit per value, D / 8 bytes (1e9 -> 125 MB)
// overlapping adds in (1) -> erase overlapping entries, insert merged (addRange, end + 1)
// reject overlapping add (LC 729) -> only check last interval starting at or before end
// return the interval holding x -> same lookup, return {start, end}; {-1, -1} if none
