// sort + mono-stack

#include <vector>
#include <algorithm>
#include <utility> // std::pair

class Solution {
public:
    int carFleet(int target, vector<int>& position, vector<int>& speed) {
        int n = static_cast<int>(position.size()); 

        std::vector<std::pair<int, int>> cars(n);
        for (int i = 0; i < n; i++) {
            cars[i] = {position[i], speed[i]};
        }
        std::sort(cars.begin(), cars.end(), std::greater<>());

        int fleets = 0;
        double lastTime = 0.0;

        for (auto& [pos, spd] : cars) {
            double curTime = static_cast<double>(target - pos) / spd;
            if (curTime > lastTime) {
                lastTime = curTime;
                fleets++;
            }
        }
        return fleets;
    }
};
