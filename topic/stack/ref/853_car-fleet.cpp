// LC 853 Car Fleet: cars drive to target on a one-lane road; position[i], speed[i]. A car
// cannot pass the car ahead; it joins it and moves at its speed (a fleet). Count fleets
// that reach target.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::sort;

// Sort by position from the front car; a car whose arrival time exceeds the fleet ahead
// can never catch it and starts a new fleet. T: O(n log n), S: O(n).
int carFleet(int target, const vector<int>& position, const vector<int>& speed) {
    int n = position.size();
    vector<pair<int, int>> cars;   // (position, speed)
    for (int i = 0; i < n; i++) { cars.push_back({position[i], speed[i]}); }
    sort(cars.begin(), cars.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    int fleets = 0;
    double lead = 0;   // arrival time of the fleet just ahead
    for (const auto& [pos, spd] : cars) {
        double time = (double)(target - pos) / spd;
        if (time > lead) {
            fleets++;
            lead = time;
        }
    }
    return fleets;
}

void test(const string& label, int target, const vector<int>& position,
          const vector<int>& speed, int want) {
    if (carFleet(target, position, speed) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("rear-catches-up", 10, {6, 8}, {5, 1}, 1);
    test("rear-too-slow", 10, {6, 8}, {3, 2}, 2);
    test("same-arrival-time", 10, {4, 8}, {3, 1}, 1);
    test("no-cars", 10, {}, {}, 0);
}
