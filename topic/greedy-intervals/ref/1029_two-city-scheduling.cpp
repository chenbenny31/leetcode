// LC 1029 Two City Scheduling: costs[i] = {aCost, bCost} to fly person i to city A or
// B. Send exactly half the people to each city; return the minimum total cost.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort by aCost - bCost: the people who gain most from A go to A, the rest to B.
// T: O(n log n), S: O(1).
int twoCitySchedCost(vector<vector<int>> costs) {
    int n = costs.size();
    sort(costs.begin(), costs.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] - a[1] < b[0] - b[1];
    });
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += i < n / 2 ? costs[i][0] : costs[i][1];
    }
    return total;
}

void test(const string& label, const vector<vector<int>>& costs, int want) {
    cout << label << (twoCitySchedCost(costs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("swap", {{1, 5}, {5, 1}}, 2);
    test("ties", {{10, 10}, {10, 10}}, 20);
    test("four", {{5, 1}, {4, 2}, {1, 9}, {2, 8}}, 6);
    test("empty", {}, 0);
}
