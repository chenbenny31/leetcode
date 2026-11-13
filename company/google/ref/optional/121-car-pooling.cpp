// 121 · Car Pooling — LC 1094
// trips[i] = [numPassengers, from, to]; car drives east only, riders on during [from, to)
// return true if load never exceeds capacity
//
// clarify:
// drop-off and pick-up at same point -> drop-off first
// location range -> 0..1000 here, difference array
// large locations -> sorted events (follow-up)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: difference array over locations: +num at from, -num at to
// running load must stay <= capacity
// T: O(n + L), S: O(L) — L = num locations
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

// follow-up:
// large coordinates -> sort events (location, delta), drop-offs first; O(n log n)
// alternative -> sort by from, min-heap of (to, num) for riders on board
// max passengers at any time -> max running load in the same sweep
// range addition (LC 370) -> same difference array, see optional 124
