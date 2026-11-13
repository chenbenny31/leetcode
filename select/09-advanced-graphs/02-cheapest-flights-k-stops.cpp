// LC 787 Cheapest Flights Within K Stops (Medium) — Bellman-Ford, k + 1 rounds
// n cities, flights[i] = [from, to, price]
// return cheapest price src -> dst w/ at most k stops (k + 1 flights), or -1
//
// clarify:
// k -> counts stops, not flights
// src == dst -> 0
// prices -> > 0
// parallel flights -> ok, min wins
// ranges -> n <= 100, price <= 1e4, int sums safe

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// core idea: Bellman-Ford, k + 1 rounds; each round relaxes all flights from prev dist
// T: O(k * E), S: O(V)
int findCheapestPrice(int n, const vector<vector<int>>& flights, int src, int dst,
                      int k) {
    // dist[city] = cheapest price to city w/ at most (round + 1) flights
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // w/o the copy one round could chain several flights
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

// follow-up:
// Dijkstra variant -> heap of (cost, city, stops); skip pops w/ stops > k
// BFS by levels -> k + 1 levels from src, best price per city, push only on improvement
// return the route -> parent[round][city] when a relax wins, walk back from dst
// no stop limit (LC 743) -> 01-network-delay (plain Dijkstra)
