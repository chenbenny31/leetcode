// LC 715 Range Module: add, remove, and query tracked half-open ranges [left, right).
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::prev;
using std::max;
using std::min;

class RangeModule {
    // start -> end of each tracked range; ranges are disjoint and never touch
    map<int, int> ranges;

public:
    // Absorb every range that overlaps or touches [left, right), then insert the union.
    // T: O(log n) amortized (each range is inserted once and erased once), S: O(n).
    void addRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second >= left) { it = prev(it); }

        while (it != ranges.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            it = ranges.erase(it);
        }
        ranges[left] = right;
    }

    // Ranges never touch, so [left, right) is tracked iff the one range starting at or
    // before left covers it. T: O(log n).
    bool queryRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it == ranges.begin()) { return false; }
        return prev(it)->second >= right;
    }

    // Erase every range overlapping [left, right), then put back the parts outside it.
    // T: O(log n) amortized.
    void removeRange(int left, int right) {
        auto it = ranges.upper_bound(left);
        if (it != ranges.begin() && prev(it)->second > left) { it = prev(it); }

        // pieces of the erased ranges that stay tracked
        vector<pair<int, int>> keep;
        while (it != ranges.end() && it->first < right) {
            if (it->first < left) { keep.push_back({it->first, left}); }
            if (it->second > right) { keep.push_back({right, it->second}); }
            it = ranges.erase(it);
        }

        for (const auto& [start, end] : keep) { ranges[start] = end; }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RangeModule r;
    r.addRange(10, 20);
    r.addRange(20, 30);
    check(r.queryRange(10, 30) && !r.queryRange(10, 31), "touching-merge");

    r.removeRange(14, 16);
    check(r.queryRange(10, 14) && r.queryRange(16, 30) && !r.queryRange(13, 17), "split");

    r.addRange(5, 40);
    check(r.queryRange(5, 40) && !r.queryRange(4, 6), "swallow");

    r.removeRange(0, 100);
    check(!r.queryRange(5, 6), "remove-all");
}
