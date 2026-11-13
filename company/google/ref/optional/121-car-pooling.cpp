// LC 1094 Car Pooling: trips[i] = [numPassengers, from, to], the car drives east only
// and passengers ride during [from, to). Return true if the load never exceeds
// capacity. 0 <= from < to <= 1000.
//
// Clarify:
// - drop-off at `to` happens before pick-up at `to` -> half-open [from, to)
// - locations bounded by 1000 -> difference array; large coordinates -> sweep
// Idea: diff[from] += num, diff[to] -= num; prefix-sum left to right and fail as soon
//   as the running load exceeds capacity.
// Pitfalls:
// - subtract at `to`, not `to + 1` (the rider is off the car at `to`)
// - size the array to max location + 1 (to can be 1000)
// Tests to write: overlap over capacity; same trips with room; drop-off and pick-up
//   at the same point; one trip larger than capacity.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// difference array over locations, then a running load. T: O(n + L), S: O(L).
bool carPooling(const vector<vector<int>>& trips, int capacity) {
    // diff[x] = change in load at location x
    vector<int> diff(1001, 0);
    for (const vector<int>& trip : trips) {
        diff[trip[1]] += trip[0];
        diff[trip[2]] -= trip[0];
    }
    int load = 0;
    for (int x = 0; x <= 1000; x++) {
        load += diff[x];
        if (load > capacity) { return false; }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& trips, int capacity,
          bool want) {
    cout << label << (carPooling(trips, capacity) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap-over", {{2, 1, 5}, {3, 3, 7}}, 4, false);
    test("overlap-fits", {{2, 1, 5}, {3, 3, 7}}, 5, true);
    test("same-point", {{2, 1, 5}, {3, 5, 7}}, 3, true);
    test("one-too-big", {{6, 0, 1}}, 5, false);
}

// follow-up: large coordinates -> sweep events (from, +num), (to, -num) sorted by
//   (time, delta) so drop-offs come first at equal times; T: O(n log n)
// follow-up: alternative -> sort by from, min-heap of (to, num) of riders on board
// follow-up: max passengers at any time -> max running load in the same sweep
// follow-up: range addition (LC 370) -> same difference array, see optional 124
