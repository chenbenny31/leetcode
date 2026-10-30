// Inventory starts at 0. On day i it changes by task[i] (> 0 receive, < 0 ship); a day
// with task[i] == 0 is a checkpoint, where inventory must be >= 0. Inventory may never
// exceed maxProducts. On the morning of any day you may restock to any higher level up to
// maxProducts. Return the min number of restock days, or -1 if impossible.
// (Same problem as Codeforces 893D "Credit Card".)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Track [lo, hi] = every inventory level reachable so far without another restock (any
// earlier restock could have topped up a bit more or less). A receipt that pushes even lo
// above the cap is fatal; the cap trims hi. At a checkpoint, if even hi < 0 one restock
// is needed: restock that morning (or earlier) to any level, so the range resets to
// [0, cap]; otherwise drop the negative part of the range. T: O(n), S: O(1).
int minRestocks(const vector<int>& task, long long maxProducts) {
    long long lo = 0, hi = 0;
    int restocks = 0;
    for (int t : task) {
        if (t == 0) {
            if (hi < 0) {
                restocks++;
                lo = 0;
                hi = maxProducts;
            } else {
                lo = max(lo, 0LL);
            }
            continue;
        }
        lo += t;
        hi += t;
        if (lo > maxProducts) { return -1; }
        hi = min(hi, maxProducts);
    }
    return restocks;
}

void test(const string& label, const vector<int>& task, long long cap, int want) {
    cout << label << (minRestocks(task, cap) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-need", {3, -2, 0}, 5, 0);
    test("one", {-1, 0}, 5, 1);
    test("reuse-range", {-2, 0, -1, 0}, 3, 1);
    test("two", {-4, 0, -5, 0}, 4, 2);
    test("overflow", {6}, 5, -1);
    test("cap-trims", {-1, 0, 3, -5, 0}, 4, 2);
}
