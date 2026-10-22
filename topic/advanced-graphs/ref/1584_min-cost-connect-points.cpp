// LC 1584 Min Cost to Connect All Points: points[i] = [x, y]; connecting two points
// costs their Manhattan distance. Return the minimum total cost to connect all points
// so that there is exactly one simple path between any two.
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <cstdlib>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::min;

// Prim on the complete graph: repeatedly add the point cheapest to attach to the tree.
// Dense graph, so the O(V^2) array version beats a heap. T: O(n^2), S: O(n).
int minCostConnectPoints(vector<vector<int>>& points) {
    int n = points.size();
    vector<int> best(n, INT_MAX);      // best[i] = cheapest edge from i into the tree
    vector<bool> inTree(n, false);
    best[0] = 0;
    int total = 0;
    for (int round = 0; round < n; round++) {
        int cur = -1;
        for (int i = 0; i < n; i++) {
            if (!inTree[i] && (cur == -1 || best[i] < best[cur])) { cur = i; }
        }
        inTree[cur] = true;
        total += best[cur];
        for (int i = 0; i < n; i++) {
            if (inTree[i]) { continue; }
            int dx = abs(points[cur][0] - points[i][0]);
            int dy = abs(points[cur][1] - points[i][1]);
            best[i] = min(best[i], dx + dy);
        }
    }
    return total;
}

void test(const string& label, vector<vector<int>> points, int want) {
    cout << label << (minCostConnectPoints(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("line", {{0, 0}, {1, 0}, {3, 0}}, 3);
    test("square", {{0, 0}, {0, 2}, {2, 0}, {2, 2}}, 6);
    test("negative-coords", {{-1, -1}, {1, 1}, {0, 0}}, 4);
    test("single", {{5, 5}}, 0);
}
