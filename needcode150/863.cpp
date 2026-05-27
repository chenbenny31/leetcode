// sort + monotonic-stack, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::sort, std::greater
#include <stack> // std::stack
#include <utility> // std::pair

class Solution {
public:
    int carFleet(int target, std::vector<int>& position,
                 std::vector<int>& speed) {
        int n = static_cast<int>(position.size());

        std::vector<std::pair<int, int>> cars(n);
        for (int i = 0; i < n; i++) { cars[i] = {position[i], speed[i]}; }
        std::sort(cars.begin(), cars.end(), std::greater<>());

        std::vector<double> buf;
        buf.reserve(n);
        std::stack<double, std::vector<double>> stk(std::move(buf));

        for (auto& [pos, spd] : cars) {
            double arrival = static_cast<double>(target - pos) / spd;
            if (stk.empty() || arrival > stk.top()) { stk.push(arrival); }
        }
        return static_cast<int>(stk.size());
    }
};

// sort by decre pos: cars closer to target proc first
// arrival > stk.top(): forms new fleet
// double for arrival time: (target - pos) / spd intro div
// drop stack use state var
int fleets = 0;
double last = 0.0;
for (auto& [pos, spd] : cars) {
    double arrival = static_cast<double>(target - pos) / spd;
    if (arrival > last) { last = arrival; fleets++; }
}
return fleets;
