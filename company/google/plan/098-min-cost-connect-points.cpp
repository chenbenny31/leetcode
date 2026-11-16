// LC 1584 Min Cost to Connect All Points (Medium). Pattern: MST, Prim O(n^2).
// points[i] = [x, y]; connecting two points costs their Manhattan distance. Return the
// minimum total cost to connect all points (a minimum spanning tree).

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

// Prim on the complete graph: keep adding the point cheapest to attach to the tree.
// Dense graph, so the array version beats a heap. T: O(n^2), S: O(n).
int minCostConnectPoints(const vector<vector<int>>& points) {
    int n = points.size();
    // best[i] = cheapest edge from i into the tree so far
    vector<int> best(n, INT_MAX);
    // inTree[i] = point i already attached
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

void test(const string& label, const vector<vector<int>>& points, int want) {
    cout << label << (minCostConnectPoints(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("line", {{0, 0}, {1, 0}, {3, 0}}, 3);
    test("square", {{0, 0}, {0, 2}, {2, 0}, {2, 2}}, 6);
    test("negative-coords", {{-1, -1}, {1, 1}, {0, 0}}, 4);
    test("single", {{5, 5}}, 0);
}

// follow-up: Kruskal -> sort all n^2 / 2 edges, add if DSU find(u) != find(v), stop at
//   n-1 edges; O(n^2 log n)
// follow-up: sparse graph -> Prim with a min-heap of (cost, node), O(E log V)
