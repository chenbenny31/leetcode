// greedy + heap

#include <vector>
#include <queue>

class Solution {
public:
    int minRefuelStops(int target, int startFuel, std::vector<std::vector<int>>& stations) {
        const int n = static_cast<int>(stations.size());
        std::vector<int> buf;
        buf.reserve(n);
        std::priority_queue<int, std::vector<int>, std::less<int>> maxHeap(std::less<int>{}, std::move(buf));

        long long reach = startFuel;
        int count = 0;
        int i = 0;

        while (reach < target) {
            while (i < n && stations[i][0] <= reach) {
                maxHeap.push(stations[i][1]);
                i++;
            }

            if (maxHeap.empty()) { return -1; }

            reach += maxHeap.top(); maxHeap.pop();
            count++;
        }
        return count;
    }
};
