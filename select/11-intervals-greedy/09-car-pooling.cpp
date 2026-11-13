// LC 1094 Car Pooling (Medium) — difference array / sweep line
// trips[i] = {passengers, from, to}, stops are 0..1000 and the car only drives forward.
// Return whether the car never holds more than capacity passengers.
// Clarify: range of the stop numbers; passengers leave at `to` before others board there;
//   trips are not sorted; number of trips.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

const int MAX_STOP = 1000;

// Core idea: record only the changes, diff[from] += passengers, diff[to] -= passengers;
// the running sum of diff is the load after each stop and must stay <= capacity.
// T: O(n + stops), S: O(stops).
bool carPooling(const vector<vector<int>>& trips, int capacity) {
    // diff[stop] = passengers boarding minus passengers leaving at that stop
    vector<int> diff(MAX_STOP + 1, 0);
    for (const vector<int>& trip : trips) {
        diff[trip[1]] += trip[0];
        // subtract at `to` itself: the seats are already free there
        diff[trip[2]] -= trip[0];
    }
    int onBoard = 0;
    for (int change : diff) {
        onBoard += change;
        if (onBoard > capacity) { return false; }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& trips, int capacity,
          bool want) {
    cout << label << (carPooling(trips, capacity) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap-over-capacity", {{2, 1, 5}, {3, 3, 7}}, 4, false);
    test("overlap-fits", {{2, 1, 5}, {3, 3, 7}}, 5, true);
    test("leave-then-board-same-stop", {{2, 1, 5}, {3, 5, 7}}, 3, true);
    test("last-stop", {{4, 0, 1000}}, 4, true);
    return 0;
}

// follow-up (ideas only):
// - flight bookings (LC 1109) -> same diff, then a prefix sum is the answer array; the
//   end is inclusive, so subtract at last + 1
// - range addition (LC 370) -> diff[start] += inc, diff[end + 1] -= inc, one prefix sum
//   at the end: O(1) per update
// - coordinates too large for an array -> sort (position, delta) events (at equal
//   positions the negative delta first) or keep the deltas in an ordered map, then sweep
// - max overlap of rectangles on a grid -> 2D difference array: +1 at (r1, c1) and
//   (r2 + 1, c2 + 1), -1 at (r1, c2 + 1) and (r2 + 1, c1), then 2D prefix sums
// - meeting rooms II (LC 253) -> 02-meeting-rooms-ii: the same sweep, the answer is the
//   max running count
// - my calendar III (LC 732) -> ordered map of deltas, re-sweep after each booking
//   (segment tree if there are many bookings)
// - range sum query 2D (LC 304) -> 2D prefix sums pre[r + 1][c + 1]; a rectangle is
//   four lookups by inclusion-exclusion
