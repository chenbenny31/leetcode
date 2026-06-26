// Prim's Algorithm: linear-scan, T: O(V2), S: O(V)

#include <vector>
#include <climits> // INT_MAX
#include <algorithm> // std::min
#include <cstdint> // uiint8_t

class Solution {
public:
    int minCostConnectPoints(std::vector<std::vector<int>>& points) {
        int n = static_cast<int>(points.size());
        std::vector<int> minDist(n, INT_MAX);
        std::vector<uint8_t> visited(n, 0);
        minDist[0] = 0;
        int total = 0;

        for (int iter = 0; iter < n; iter++) {
            // find un-visited node with min dist
            int u = -1;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && (u == -1 || minDist[i] < minDist[u])) { u = i; }
            }
            visited[u] = 1;
            total += minDist[u];

            // update dist to un-visited neighbors
            for (int v = 0; v < n; v++) {
                if (visited[v]) { continue; }
                int d = std::abs(points[u][0] - points[v][0]) +
                        std::abs(points[u][1] - points[v][1]);
                minDist[v] = std::min(minDist[v], d);
            }
        }
        return total;
    }
};

// Kruskal's Algorithm, T: O(ElogE), S: O(V+E)

#include <vector>
#include <algorithm> // std::sort
#include <utility> // std::swap
#include <tuple> 

class Solution {
public:
    int minCostConnectPoints(std::vector<std::vector<int>>& points) {
        int n = static_cast<int>(points.size());

        std::vector<std::tuple<int, int, int>> edges;
        edges.reserve(n * (n-1) / 2);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int d = std::abs(points[i][0] - points[j][0]) +
                        std::abs(points[i][1] - points[j][1]);
                edges.push_back({d, i, j});
            }
        }
        std::sort(edges.begin(), edges.end());

        std::vector<int> parent(n), rnk(n, 0);
        for (int i = 0; i < n; i++) { parent[i] = i; }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        int total = 0, cnt = 0;
        for (auto& [d, u, v] : edges) {
            int pu = find(u), pv = find(v);
            if (pu == pv) { continue; }
            if (rnk[pu] < rnk[pv]) { std::swap(pu, pv); }
            parent[pv] = pu;
            if (rnk[pu] == rnk[pv]) { rnk[pu]++; }
            total += d;
            ++cnt;
            if (cnt == n - 1) { break; }
        }
        return total;
    }
};

// Prim's vs Kruskal's: dense graph prefers Prim's with linear-scan, sparse graph prefers Kruskal's
// Prim's: no priority queue for dense graph (O(ElogV))
// Kruskal's early exit: cnt == n - 1, as MST has exactly n - 1 edges
