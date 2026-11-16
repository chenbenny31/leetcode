// LC 1094 Car Pooling (Medium) — difference array / sweep line
// trips[i] = {passengers, from, to}; stops 0..1000, car only drives forward
// return if load never exceeds capacity
//
// clarify:
// stop range -> 0..1000, array fits; larger -> sorted events
// leave at `to` before others board -> subtract at `to` itself
// trips unsorted -> ok, diff array needs no order
// num of trips -> O(n + stops) either way

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

const int MAX_STOP = 1000;

// core idea: diff[from] += p, diff[to] -= p; running sum = load, must stay <= capacity
// T: O(n + stops), S: O(stops)
bool carPooling(const vector<vector<int>>& trips, int capacity) {
    // diff[stop] = boarding - leaving at that stop
    vector<int> diff(MAX_STOP + 1, 0);
    for (const vector<int>& trip : trips) {
        diff[trip[1]] += trip[0];
        // subtract at `to` itself: seats already free there
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

// follow-up:
// flight bookings (LC 1109) -> same diff, prefix sum = answer; end inclusive -> last + 1
// range addition (LC 370) -> diff[start] += inc, diff[end + 1] -= inc, one prefix sum
// coords too large -> sort (pos, delta) events, negative delta first on ties, sweep
// max overlap of rectangles -> 2D diff array at the 4 corners, then 2D prefix sums
// meeting rooms II (LC 253) -> same sweep, answer = max running cnt
// my calendar III (LC 732) -> ordered map of deltas, re-sweep after each booking
// range sum query 2D (LC 304) -> 2D prefix sums pre[r + 1][c + 1], 4 lookups
