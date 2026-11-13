// LC 787 Cheapest Flights Within K Stops (Medium) — Bellman-Ford, k + 1 rounds
// n cities, flights[i] = [from, to, price]. Return the cheapest price from src to dst
// using at most k stops (k + 1 flights), or -1 if there is no such route.
// Clarify: k counts stops, not flights; src == dst; prices > 0; parallel flights;
//   ranges (n <= 100, price <= 1e4, so int sums are safe).

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Core idea: Bellman-Ford for k + 1 rounds; each round relaxes every flight from a copy
// of the previous round's dist, so a route grows by one flight per round.
// T: O(k * E), S: O(V).
int findCheapestPrice(int n, const vector<vector<int>>& flights, int src, int dst,
                      int k) {
    // dist[city] = cheapest price to city using at most (round + 1) flights
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // without the copy one round could chain several flights in edge order
        vector<int> prev = dist;
        for (const vector<int>& flight : flights) {
            int from = flight[0];
            int to = flight[1];
            int price = flight[2];
            if (prev[from] == INT_MAX) { continue; }
            dist[to] = min(dist[to], prev[from] + price);
        }
    }
    return dist[dst] == INT_MAX ? -1 : dist[dst];
}

void test(const string& label, int n, const vector<vector<int>>& flights, int src,
          int dst, int k, int want) {
    bool ok = findCheapestPrice(n, flights, src, dst, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-stop", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}}, 0, 2, 1, 200);
    test("zero-stops", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 500}}, 0, 2, 0, 500);
    test("copy-matters", 4, {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {0, 3, 10}}, 0, 3, 1, 10);
    test("unreachable", 2, {}, 0, 1, 1, -1);
    return 0;
}

// follow-up (ideas only):
// - Dijkstra variant -> heap of (cost, city, stops); skip a pop whose stops exceed k and
//   keep the fewest stops seen per city to prune; O(k * E log(k * V))
// - BFS by levels -> k + 1 levels from src, keeping the best price per city and pushing
//   only on improvement; the same work as the rounds above
// - return the route -> keep parent[round][city] when a relax wins, walk back from dst
// - no stop limit (LC 743) -> 01-network-delay (plain Dijkstra)
