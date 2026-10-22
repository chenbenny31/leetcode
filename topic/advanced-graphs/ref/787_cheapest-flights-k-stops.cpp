// LC 787 Cheapest Flights Within K Stops: flights[i] = [from, to, price] on n cities.
// Return the cheapest price from src to dst with at most k stops, or -1 if impossible.
#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// Bellman-Ford limited to k+1 rounds (k stops = k+1 flights). T: O(k*E), S: O(V).
int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
    // dist[c] = cheapest price to c using at most `round` flights
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // relax from last round's copy so a route grows by one flight per round
        vector<int> prev = dist;
        for (const auto& f : flights) {
            if (prev[f[0]] == INT_MAX) { continue; }
            dist[f[1]] = min(dist[f[1]], prev[f[0]] + f[2]);
        }
    }
    return dist[dst] == INT_MAX ? -1 : dist[dst];
}

void test(const string& label, int n, vector<vector<int>> flights, int src, int dst,
          int k, int want) {
    cout << label << (findCheapestPrice(n, flights, src, dst, k) == want ? " PASS\n"
                                                                         : " FAIL\n");
}

int main() {
    test("direct-vs-stop", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 300}}, 0, 2, 1, 200);
    test("stop-limit", 3, {{0, 1, 100}, {1, 2, 100}, {0, 2, 300}}, 0, 2, 0, 300);
    test("unreachable", 2, {{1, 0, 5}}, 0, 1, 3, -1);
    test("same-round-trap", 4, {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {0, 3, 9}}, 0, 3, 1, 9);
}
