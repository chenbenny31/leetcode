// LC 743 Network Delay Time (Medium). Pattern: Dijkstra with a min-heap.
// Directed edges times[i] = [u, v, w] on nodes 1..n. A signal starts at node k; return
// the time for all nodes to receive it, or -1 if some never do.
//
// Clarify:
// - nodes are 1..n, not 0..n-1 -> arrays of size n + 1
// - directed edges, weights >= 0 (negative weights -> Bellman-Ford, follow-up)
// - parallel edges? -> fine, the cheaper one wins on relax
// - some node never reached -> -1
// - ranges (n <= 100, E <= 6000, w <= 100) -> int sums cannot overflow
// Idea: Dijkstra from k with a min-heap of (time, node): with weights >= 0 the first pop
//   of a node is its shortest time. The answer is the largest shortest time.
// Pitfalls:
// - the default priority_queue is a max-heap: pass greater<pair<int, int>>
// - (time, node) order in the pair, so the heap sorts by time
// - skip stale entries (curTime > dist[cur]); without it the answer is right but slow
// - relax only from a popped node: its dist is finite, so INT_MAX + w never happens
// - 1-indexed: graph(n + 1), final loop v = 1..n
// Tests to write: diamond (the cheaper of two paths wins); two short edges beat one
//   long direct edge; unreachable node -> -1; single node -> 0.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <tuple>
#include <climits>
#include <cmath>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::tuple;
using std::priority_queue;
using std::greater;
using std::max;
using std::abs;

// Dijkstra from k; the answer is the farthest shortest distance.
// T: O((V+E) log V), S: O(V+E).
int networkDelayTime(const vector<vector<int>>& times, int n, int k) {
    // graph[u] = (v, w) for each edge u -> v taking w
    vector<vector<pair<int, int>>> graph(n + 1);
    for (const vector<int>& edge : times) {
        graph[edge[0]].push_back({edge[1], edge[2]});
    }
    // dist[v] = best known time from k to v
    vector<int> dist(n + 1, INT_MAX);
    // (time, node), earliest first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[k] = 0;
    pq.push({0, k});
    while (!pq.empty()) {
        auto [curTime, cur] = pq.top();
        pq.pop();
        // stale entry: cur was already settled with a smaller time
        if (curTime > dist[cur]) { continue; }
        for (const auto& [nxt, delay] : graph[cur]) {
            int nxtTime = curTime + delay;
            if (nxtTime < dist[nxt]) {
                dist[nxt] = nxtTime;
                pq.push({nxtTime, nxt});
            }
        }
    }
    int result = 0;
    for (int v = 1; v <= n; v++) {
        if (dist[v] == INT_MAX) { return -1; }
        result = max(result, dist[v]);
    }
    return result;
}

// LC 1514: undirected edges[i] = [u, v] succeed with succProb[i]; max probability of a
// path start -> end, 0 if none. Dijkstra with a max-heap: products only shrink along a
// path, so the first pop of end is the best. T: O((V+E) log V), S: O(V+E).
// Pitfalls: here the default max-heap is what you want; best starts at 0.0, not -1;
// return on the first pop of end, not the first push.
double maxProbability(int n, const vector<vector<int>>& edges,
                      const vector<double>& succProb, int start, int end) {
    // graph[u] = (v, p) for each edge u - v succeeding with probability p
    vector<vector<pair<int, double>>> graph(n);
    int numEdges = edges.size();
    for (int i = 0; i < numEdges; i++) {
        graph[edges[i][0]].push_back({edges[i][1], succProb[i]});
        graph[edges[i][1]].push_back({edges[i][0], succProb[i]});
    }
    // best[v] = highest known probability from start to v
    vector<double> best(n, 0.0);
    // (probability, node), most likely first
    priority_queue<pair<double, int>> pq;
    best[start] = 1.0;
    pq.push({1.0, start});
    while (!pq.empty()) {
        auto [curProb, cur] = pq.top();
        pq.pop();
        if (cur == end) { return curProb; }
        // stale entry: cur was already settled with a higher probability
        if (curProb < best[cur]) { continue; }
        for (const auto& [nxt, prob] : graph[cur]) {
            double nxtProb = curProb * prob;
            if (nxtProb > best[nxt]) {
                best[nxt] = nxtProb;
                pq.push({nxtProb, nxt});
            }
        }
    }
    return 0.0;
}

// LC 3970: directed edges [u, v, w], labels[v] = lowercase label of node v, k >= 1.
// Min total weight of a path 0 -> n - 1 whose label sequence has no run of more than k
// equal consecutive chars, -1 if none. Dijkstra over states (node, run), run = length
// of the label run ending at node. Plain dist[node] is not enough: the cheapest arrival
// may have a long run that cannot continue, while a pricier one with a shorter run is
// the only one that can. T: O(k (V+E) log(k E)), S: O(k (V+E)).
// Pitfall: whenever a constraint depends on how you arrived (run length, stops used,
// breaks left), put it in the state and in dist.
long long shortestPathRunLimit(int n, const vector<vector<int>>& edges,
                               const string& labels, int k) {
    // graph[u] = (v, w) for each edge u -> v of weight w
    vector<vector<pair<int, int>>> graph(n);
    for (const auto& edge : edges) { graph[edge[0]].push_back({edge[1], edge[2]}); }
    // dist[v][run] = min weight to reach v with a label run of length run ending at v
    vector<vector<long long>> dist(n, vector<long long>(k + 1, LLONG_MAX));
    // (weight, node, run), cheapest first
    priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>,
                   greater<tuple<long long, int, int>>> pq;
    dist[0][1] = 0;
    pq.push({0, 0, 1});
    while (!pq.empty()) {
        auto [curDist, cur, run] = pq.top();
        pq.pop();
        // first pop of the target is the cheapest over all its runs
        if (cur == n - 1) { return curDist; }
        if (curDist > dist[cur][run]) { continue; }
        for (const auto& [nxt, weight] : graph[cur]) {
            // same label extends the run, a different one starts a new run
            int nxtRun = labels[nxt] == labels[cur] ? run + 1 : 1;
            if (nxtRun > k) { continue; }
            long long nxtDist = curDist + weight;
            if (nxtDist < dist[nxt][nxtRun]) {
                dist[nxt][nxtRun] = nxtDist;
                pq.push({nxtDist, nxt, nxtRun});
            }
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& times, int n, int k,
          int want) {
    cout << label << (networkDelayTime(times, n, k) == want ? " PASS\n" : " FAIL\n");
}

void testProb(const string& label, int n, const vector<vector<int>>& edges,
              const vector<double>& succProb, int start, int end, double want) {
    bool ok = abs(maxProbability(n, edges, succProb, start, end) - want) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testRun(const string& label, int n, const vector<vector<int>>& edges,
             const string& labels, int k, long long want) {
    bool ok = shortestPathRunLimit(n, edges, labels, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2, 1}, {1, 3, 4}, {2, 4, 2}, {3, 4, 1}}, 4, 1, 4);
    test("shortcut", {{1, 2, 5}, {1, 3, 1}, {3, 2, 1}}, 3, 1, 2);
    test("unreachable", {{1, 2, 1}}, 3, 1, -1);
    test("single", {}, 1, 1, 0);
    testProb("prob-two-hops-win", 3, {{0, 1}, {1, 2}, {0, 2}}, {0.5, 0.5, 0.2}, 0, 2,
             0.25);
    testProb("prob-direct-wins", 3, {{0, 1}, {1, 2}, {0, 2}}, {0.5, 0.5, 0.3}, 0, 2,
             0.3);
    testProb("prob-unreachable", 3, {{0, 1}}, {0.5}, 0, 2, 0.0);
    testRun("run-detour-wins", 4, {{0, 2, 1}, {0, 1, 1}, {1, 2, 1}, {2, 3, 1}}, "babb",
            2, 3);
    testRun("run-direct-ok", 4, {{0, 2, 1}, {0, 1, 1}, {1, 2, 1}, {2, 3, 1}}, "babb",
            3, 2);
    testRun("run-blocked", 2, {{0, 1, 5}}, "aa", 1, -1);
    testRun("run-single-node", 1, {}, "a", 1, 0);
}

// follow-up: return the path, not only the time -> prev[nxt] = cur whenever dist[nxt]
//   improves (prev[k] = -1), then walk back from the target and reverse:
//   for (int v = target; v != -1; v = prev[v]) { path.push_back(v); }
//   reverse(path.begin(), path.end());
// follow-up: at most k stops (LC 787, flights) -> Bellman-Ford for k + 1 rounds, each
//   round relaxing every edge from a copy of the previous round's dist:
//   select/optional/09-advanced-graphs/02-cheapest-flights-k-stops.cpp
// follow-up: negative edge weights -> Bellman-Ford, n - 1 rounds relaxing every edge:
//   for (int round = 0; round < n - 1; round++) {
//       for (const auto& e : times) {
//           if (dist[e[0]] == INT_MAX) { continue; }
//           dist[e[1]] = min(dist[e[1]], dist[e[0]] + e[2]);
//       }
//   }
//   O(V*E); an n-th round that still lowers a dist = negative cycle
// follow-up: all weights 1 -> plain BFS; weights 0 or 1 -> 0-1 BFS (deque: push_front
//   on a 0 edge, push_back on a 1 edge), O(V+E)
// follow-up: dense graph -> O(V^2) array Dijkstra (pick min unvisited each round)
