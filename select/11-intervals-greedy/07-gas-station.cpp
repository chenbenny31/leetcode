// LC 134 Gas Station (Medium). Pattern: one-pass greedy, reset start on negative tank.
// Circular route: gas[i] at station i, cost[i] to drive to the next one. Return the
// start index that completes the loop (unique if it exists), else -1.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// If the tank goes negative at i, no start in [start, i] works, so restart at i + 1.
// A start exists iff the total net gas >= 0. T: O(n), S: O(1).
int canCompleteCircuit(const vector<int>& gas, const vector<int>& cost) {
    int n = gas.size();
    // net gas over the whole loop
    int total = 0;
    // net gas since the current start
    int tank = 0;
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
    test("start-last", {1, 1, 5}, {2, 2, 1}, 2);
    test("start-middle", {1, 4, 2}, {3, 1, 2}, 1);
    test("not-enough", {1, 2}, {2, 2}, -1);
    test("single", {5}, {4}, 0);
}

// follow-up: why total >= 0 suffices -> the part before start has net < 0, so the part
//   from start to the end carries more than enough to cover it after wrapping
