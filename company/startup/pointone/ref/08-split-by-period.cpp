// Practical: a time entry is [start, end) in minutes from a common origin. Split it at
// every period boundary (1440 = midnight, or a billing-period length) so each piece
// lands in exactly one day / period. Return the pieces in order.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Walk from start, cutting at the next multiple of period each time.
// T: O(pieces), S: O(pieces).
vector<vector<int>> splitByPeriod(int start, int end, int period) {
    vector<vector<int>> pieces;   // [from, to) pieces, each inside one period
    int cur = start;
    while (cur < end) {
        // first boundary strictly after cur
        int boundary = (cur / period + 1) * period;
        int to = min(end, boundary);
        pieces.push_back({cur, to});
        cur = to;
    }
    return pieces;
}

void test(const string& label, int start, int end, const vector<vector<int>>& want) {
    cout << label << (splitByPeriod(start, end, 1440) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("within", 60, 120, {{60, 120}});
    test("cross", 1380, 1500, {{1380, 1440}, {1440, 1500}});
    test("multi", 0, 3000, {{0, 1440}, {1440, 2880}, {2880, 3000}});
    test("on-edge", 1440, 1500, {{1440, 1500}});
}
