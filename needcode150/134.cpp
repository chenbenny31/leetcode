// greedy one-pass, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    int canCompleteCircuit(std::vector<int>& gas, std::vector<int>& cost) {
        int n = static_cast<int>(gas.size());
        int totalTank = 0;
        int curTank = 0;
        int start = 0;

        for (int i = 0; i < n; i++) {
            int diff = gas[i] - cost[i];
            totalTank += diff;
            curTank += diff;
            if (curTank < 0) {
                start = i + 1;
                curTank = 0;
            }
        }
        return totalTank >= 0 ? start : -1;
    }
};

// resetting on failure: if negative arrive at i starting from start, every [start, i] fail as well
