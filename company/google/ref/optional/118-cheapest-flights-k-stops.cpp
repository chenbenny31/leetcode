// 118 · Cheapest Flights Within K Stops — LC 787
// Mother: select/09-advanced-graphs/02-cheapest-flights-k-stops.cpp
// n cities, directed flights [from, to, price]; cheapest price src -> dst w/ <= k stops
// return -1 if no such route
//
// clarify:
// k stops -> k + 1 flights
// prices -> non-negative
// src == dst -> 0
// no route -> -1

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// core idea: Bellman-Ford w/ k + 1 rounds; relax every flight from last round's copy
// copy -> a path grows by at most one flight per round
// T: O(k * E), S: O(V)
int findCheapestPrice(int n, const vector<vector<int>>& flights, int src, int dst,
                      int k) {
    // dist[city] = cheapest price to city using at most (round + 1) flights
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // last round's dist: without the copy one round could chain several flights
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
}

// follow-up:
// Dijkstra variant -> heap of (cost, city, stops), skip stops > k
// prune Dijkstra -> keep fewest stops seen per city
