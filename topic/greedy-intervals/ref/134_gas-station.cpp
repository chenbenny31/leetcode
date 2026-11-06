// LC 134 Gas Station: on a circular route, return the unique start station that completes
// the loop from an empty tank, or -1.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Greedy one pass: if the tank goes negative at i, no start in [start, i] works, so
// restart at i + 1. A start exists iff total >= 0. T: O(n), S: O(1).
int canCompleteCircuit(const vector<int>& gas, const vector<int>& cost) {
    int n = gas.size();
    int total = 0;   // net gas over the whole loop
    int tank = 0;    // net gas since the current start
    int start = 0;

    for (int i = 0; i < n; i++) {
        total += gas[i] - cost[i];
        tank += gas[i] - cost[i];
        if (tank < 0) {
            start = i + 1;
            tank = 0;
        }
    }

    return total < 0 ? -1 : start;
}

void test(const string& label, const vector<int>& gas, const vector<int>& cost,
          int want) {
    cout << label << (canCompleteCircuit(gas, cost) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("wrap-around", {1, 2, 3, 4, 5}, {3, 4, 5, 1, 2}, 3);
    test("not-enough", {2, 3, 4}, {3, 4, 3}, -1);
    test("single", {5}, {4}, 0);
}
