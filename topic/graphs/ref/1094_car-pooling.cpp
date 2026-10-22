// LC 1094 Car Pooling: trips[i] = [passengers, from, to] along a one-way road; the car
// holds `capacity` passengers. Passengers get off at `to` before others get on there.
// Can all trips be done?
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Difference array over stops, then a running sum of passengers on board.
// T: O(N + S), S: O(S); N = trips, S = last stop.
bool carPooling(const vector<vector<int>>& trips, int capacity) {
    int last = 0;
    for (const auto& t : trips) { last = max(last, t[2]); }
    vector<int> diff(last + 1, 0);   // diff[s] = change in passengers on board at stop s
    for (const auto& t : trips) {
        diff[t[1]] += t[0];
        diff[t[2]] -= t[0];
    }
    int onboard = 0;
    for (int d : diff) {
        onboard += d;
        if (onboard > capacity) { return false; }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& trips, int capacity,
          bool want) {
    cout << label << (carPooling(trips, capacity) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overload", {{2, 1, 5}, {3, 3, 7}}, 4, false);
    test("drop-before-pickup", {{2, 1, 5}, {3, 5, 7}}, 3, true);
    test("no-trips", {}, 1, true);
}
