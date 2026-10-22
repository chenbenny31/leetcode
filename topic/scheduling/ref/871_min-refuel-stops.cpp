// LC 871 Minimum Number of Refueling Stops: a car with startFuel drives to target,
// one fuel unit per mile; stations[i] = {position, fuel}. Return the fewest stops, or -1.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;

// Drive as far as possible; only when stuck, retroactively refuel at the biggest station
// already passed. T: O(n log n), S: O(n).
int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
    int n = stations.size();
    priority_queue<int> passed;   // fuel of passed stations not yet used
    long long reach = startFuel;  // farthest position reachable so far
    int stops = 0;
    int i = 0;
    while (reach < target) {
        while (i < n && stations[i][0] <= reach) {
            passed.push(stations[i][1]);
            i++;
        }
        if (passed.empty()) { return -1; }
        reach += passed.top();
        passed.pop();
        stops++;
    }
    return stops;
}

void test(const string& label, int target, int startFuel, vector<vector<int>> stations,
          int want) {
    cout << label << (minRefuelStops(target, startFuel, stations) == want ? " PASS\n"
                                                                          : " FAIL\n");
}

int main() {
    test("two-stops", 50, 10, {{5, 20}, {10, 15}, {30, 40}}, 2);
    test("no-stop", 10, 10, {}, 0);
    test("unreachable", 100, 10, {{20, 100}}, -1);
}
