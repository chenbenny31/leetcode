// 33 · Currency arbitrage — quant core (reported at Citadel Securities, not here)
// rate {from, to, r} means 1 unit of `from` buys r units of `to`; is there a cycle of
// conversions that ends with more than it started with? return one such cycle (the
// currencies in conversion order), {} if there is none
//
// clarify:
// currencies -> 0 .. n - 1; every rate > 0; a pair may be missing or one-way
// which cycle -> any profitable one, not the best; no fees
// product of rates == 1 (a fair round trip) -> not arbitrage

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::reverse;
using std::log;

struct Rate {
    int from;
    int to;
    double rate; // 1 unit of from buys this many units of to
};

// core idea: profit <=> product of rates > 1 <=> sum of -log(rate) < 0, so with edge
// weight -log(rate) an arbitrage is a negative cycle -> Bellman-Ford; dist starts at 0
// for every currency, so a cycle is found wherever it sits in the graph
// T: O(V * E), S: O(V), V = num of currencies, E = num of rates
vector<int> findArbitrage(int n, const vector<Rate>& rates) {
    vector<double> dist(n, 0);  // dist[c] = cheapest path cost found so far ending at c
    vector<int> parent(n, -1);  // parent[c] = currency right before c on that path
    int lastChanged = -1;       // a currency that got cheaper in the latest round

    // without a negative cycle nothing changes after n - 1 rounds
    for (int round = 0; round < n; round++) {
        lastChanged = -1;
        for (const Rate& edge : rates) {
            double cost = dist[edge.from] - log(edge.rate);
            // 1e-12: ignore float noise
            if (cost < dist[edge.to] - 1e-12) {
                dist[edge.to] = cost;
                parent[edge.to] = edge.from;
                lastChanged = edge.to;
            }
        }
        if (lastChanged == -1) { return {}; }
    }

    // still changing in round n -> lastChanged hangs off a negative cycle; n steps
    // back along parent always land inside the cycle
    int inCycle = lastChanged;
    for (int i = 0; i < n; i++) { inCycle = parent[inCycle]; }

    vector<int> cycle = {inCycle};
    for (int cur = parent[inCycle]; cur != inCycle; cur = parent[cur]) {
        cycle.push_back(cur);
    }
    // parent goes backward, we want conversion order
    reverse(cycle.begin(), cycle.end());
    return cycle;
}

// rate from -> to, 0 if there is none
double rateOf(const vector<Rate>& rates, int from, int to) {
    for (const Rate& edge : rates) {
        if (edge.from == from && edge.to == to) { return edge.rate; }
    }
    return 0;
}

// several cycles can be right: check that the returned one really makes money
void test(const string& label, int n, const vector<Rate>& rates, bool wantCycle) {
    vector<int> cycle = findArbitrage(n, rates);
    int len = cycle.size();
    double product = 1;
    for (int i = 0; i < len; i++) {
        product *= rateOf(rates, cycle[i], cycle[(i + 1) % len]);
    }
    bool ok = wantCycle ? (len >= 2 && product > 1) : cycle.empty();
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("triangle", 3, {{0, 1, 0.9}, {1, 2, 0.9}, {2, 0, 1.3}}, true);
    test("fair-round-trip", 2, {{0, 1, 0.5}, {1, 0, 2.0}}, false);
    test("bid-ask-spread", 2, {{0, 1, 0.9}, {1, 0, 1.1}}, false);
    test("cycle-away-from-0", 4, {{0, 1, 0.5}, {2, 3, 2.0}, {3, 2, 0.6}}, true);
}

// follow-up:
// 1. only yes / no -> drop parent; true when round n still relaxes an edge
// 2. best rate from A to B (no arbitrage) -> max product path: the same -log weights
// 3. fees -> multiply each rate by (1 - fee) before taking the log
// 4. rates as a matrix -> Floyd-Warshall on products; arbitrage if best[i][i] > 1
// 5. rates stream in -> rerun from the changed edge's endpoints, or check triangles only
