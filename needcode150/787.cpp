// Bellman-Ford with swap+move, O(kE), S: O(V)

#include <vector>
#include <algorithm> // std::min, std::swap
#include <climits> // INT_MAX

class Solution {
public:
    int findCheapestPrice(int n, std::vector<std::vector<int>>& flights, int src, int dst, int k) {
        std::vector<int> cost(n, INT_MAX);
        std::vector<int> tmp(n);
        cost[src] = 0;

        for (int i = 0; i <= k; i++) {
            tmp = cost; // deep copy - snapshot of previous iteration
            for (auto& f : flights) {
                int u = f[0], v = f[1], wt = f[2];
                if (cost[u] == INT_MAX) { continue; }
                tmp[v] = std::min(tmp[v], cost[u] + wt);
            }
            std::swap(cost, tmp);
        }
        return cost[dst] == INT_MAX ? -1 : cost[dst];
    }
};

// Dijskstra with stop-counting pruning, T: O(ElogV), S: O(V+E)

#include <vector>
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <tuple>
#include <functional> // std::greater
#include <climits> // INT_MAX

class Solution {
public:
    int findCheapestPrice(int n, std::vector<std::vector<int>>& flights,
                          int src, int dst, int k) {
        std::vector<std::vector<std::pair<int, int>>> adj(n);
        for (auto& f : flights) { adj[f[0]].push_back({f[1], f[2]}); }

        std::vector<std::tuple<int, int, int>> buf;
        buf.reserve(flights.size());
        std::priority_queue<std::tuple<int, int, int>,
                            std::vector<std::tuple<int, int, int>>,
                            std::greater<>> pq(std::greater<>{}, std::move(buf));
        pq.push({0, src, 0}); // {cost, node, stops}
        std::vector<int> minStops(n, INT_MAX);

        while (!pq.empty()) {
            auto [cost, node, stops] = pq.top(); pq.pop();
            if (node == dst) { return cost; }
            if (stops > k) { continue; }
            if (minStops[node] <= stops) { continue; }
            minStops[node] = stops;

            for (auto& [nxt, wt] : adj[node]) {
                pq.push({cost + wt, nxt, stops + 1});
            }
        }
        return -1;
    }
};

// tmp = cost must be deep copy: relaxing need prev and curr state comparasion
// std::swap(cost, tmp) over cost = tmp: avoid a second un-necessary copy
// i <= k not i < k: k stops = k+1 edges, loop must run k+1 times
// BellmanFord vs Dijkstra: for small k and large E, BF's fixed iterations wins; for large k and sparse graphs, Dijkstra has early exit
