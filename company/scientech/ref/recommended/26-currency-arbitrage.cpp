// currency arbitrage: rate {from, to, r} means 1 unit of `from` buys r units of `to`
// is there a cycle of conversions that ends with more than it started with?
// return one such cycle (the currencies in conversion order), {} if there is none

// usage:
// g++ -std=c++17 -Wall -Wextra 26-currency-arbitrage.cpp -o a && ./a
//
// cycle = findArbitrage(numCurrencies, rates);

// clarify / assumptions
// currencies are 0 .. n - 1; every rate > 0; a pair may be missing or one-way
// any profitable cycle is fine, not the best one; no fees
// product of rates == 1 (a fair round trip) is not arbitrage

// idea:
// profit <=> product of rates on the cycle > 1
//        <=> sum of log(rate) > 0  <=>  sum of -log(rate) < 0
// so: edge weight = -log(rate), and arbitrage = a negative cycle -> Bellman-Ford
// dist starts at 0 for every currency: a cycle is found wherever it is in the graph

// T: O(V * E), S: O(V), V = num of currencies, E = num of rates

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

using std::log;
using std::reverse;
using std::cout;
using std::string;
using std::vector;

struct Rate {
    int from;
    int to;
    double rate; // 1 unit of from buys this many units of to
};

vector<int> findArbitrage(int n, const vector<Rate>& rates) {
    vector<double> dist(n, 0); // dist[c] = cheapest path cost found so far ending at c
    vector<int> parent(n, -1); // parent[c] = currency right before c on that path
    int lastChanged = -1; // a currency that got cheaper in the latest round

    // without a negative cycle nothing changes after n - 1 rounds
    for (int round = 0; round < n; round++) {
        lastChanged = -1;
        for (const Rate& edge : rates) {
            double cost = dist[edge.from] - log(edge.rate);
            if (cost < dist[edge.to] - 1e-12) { // 1e-12: ignore float noise
                dist[edge.to] = cost;
                parent[edge.to] = edge.from;
                lastChanged = edge.to;
            }
        }
        if (lastChanged == -1) { return {}; } // settled: no negative cycle
    }

    // still changing in round n -> lastChanged hangs off a negative cycle;
    // n steps back along parent always lands inside the cycle
    int inCycle = lastChanged;
    for (int i = 0; i < n; i++) {
        inCycle = parent[inCycle];
    }

    vector<int> cycle;
    cycle.push_back(inCycle);
    int cur = parent[inCycle];
    while (cur != inCycle) {
        cycle.push_back(cur);
        cur = parent[cur];
    }
    reverse(cycle.begin(), cycle.end()); // parent goes backward, we want trade order
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
    bool ok = cycle.empty();
    if (wantCycle) {
        double product = 1;
        int len = cycle.size();
        for (int i = 0; i < len; i++) {
            product *= rateOf(rates, cycle[i], cycle[(i + 1) % len]);
        }
        ok = (len >= 2 && product > 1);
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // 0 = USD, 1 = EUR, 2 = GBP
    test("triangle", 3, {{0, 1, 0.9}, {1, 2, 0.9}, {2, 0, 1.3}}, true);
    test("fair-round-trip", 2, {{0, 1, 0.5}, {1, 0, 2.0}}, false);
    test("bid-ask-spread", 2, {{0, 1, 0.9}, {1, 0, 1.1}}, false);
    test("cycle-away-from-0", 4, {{0, 1, 0.5}, {2, 3, 2.0}, {3, 2, 0.6}}, true);
    test("no-rates", 3, {}, false);
}
