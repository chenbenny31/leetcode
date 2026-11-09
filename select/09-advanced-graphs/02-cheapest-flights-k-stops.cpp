// LC 787 Cheapest Flights Within K Stops (Medium). Pattern: Bellman-Ford, k+1 rounds.
// n cities, flights[i] = [from, to, price]. Return the cheapest price from src to dst
// with at most k stops, or -1 if there is no such route.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Bellman-Ford with k+1 rounds (k stops = k+1 edges). T: O(k*E), S: O(V).
int findCheapestPrice(int n, const vector<vector<int>>& flights, int src, int dst,
                      int k) {
    // dist[city] = cheapest price to city using at most (round + 1) flights
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    for (int round = 0; round <= k; round++) {
        // relax from last round's copy so a path grows by at most one edge per round
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

// follow-up: why the copy -> without prev, one round can chain several edges in edge
//   order and use more than k+1 flights
// follow-up: Dijkstra variant -> heap of (cost, city, stops); skip when stops > k; keep
//   best stops per city to prune, O(k * E log(k*V))
