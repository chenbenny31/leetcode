// LC 2276 Count Integers in Intervals: add(left, right) adds every integer in
// [left, right] to a set; count() returns how many integers the set holds.
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
using std::cout;
using std::map;
using std::max;
using std::min;
using std::prev;
using std::string;

class CountIntervals {
    // start -> end (inclusive) of the disjoint ranges that cover the set
    map<int, int> ranges;
    int total = 0;   // integers covered = sum of range lengths

public:
    // merge every range that overlaps the new one into it; each range is erased once.
    // T: O(log n) amortized, S: O(n) ranges.
    void add(int left, int right) {
        auto it = ranges.lower_bound(left);
        // the previous range may reach into [left, right]
        if (it != ranges.begin() && prev(it)->second >= left) { it = prev(it); }
        while (it != ranges.end() && it->first <= right) {
            left = min(left, it->first);
            right = max(right, it->second);
            total -= it->second - it->first + 1;
            it = ranges.erase(it);
        }
        ranges[left] = right;
        total += right - left + 1;
    }

    int count() {
        return total;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    CountIntervals c;
    check(c.count() == 0, "empty");
    c.add(2, 3);
    c.add(7, 10);
    check(c.count() == 6, "disjoint");
    c.add(5, 8);
    check(c.count() == 8, "overlap-right");
    c.add(1, 20);
    check(c.count() == 20, "swallow-all");
}
