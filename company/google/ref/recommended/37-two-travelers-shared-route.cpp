// Two Travelers Shared Route (Google, interview report; LC 2203 style). n cities 0..n-1,
// undirected unweighted roads (assumed; weighted -> follow-up). Travelers start at a and
// b and must reach dest; they may meet at a city m and ride together from m to dest, the
// shared part counted once. Return min dist(a, m) + dist(b, m) + dist(m, dest) over all
// m (m may be a, b or dest itself), or -1 if no m works.
//
// Clarify:
// - roads two-way and unweighted -> BFS; weighted or one-way roads are follow-ups
// - m may be a, b or dest, and a == b is allowed -> those terms are just 0
// - V, E up to ~1e5 -> three BFS in O(V + E), not one BFS per candidate m
// - parallel roads, self-loops -> harmless for BFS
// - no city that a and b both reach and that reaches dest -> -1
// - return only the total length (the meeting city is a follow-up)
// Idea: once the two routes meet they never need to split again, so the best answer is
//   a "Y": a -> m, b -> m, m -> dest. BFS from a, from b and from dest gives all three
//   terms for every m at once; take the min sum over m.
// Pitfalls:
// - skip m when any of the three distances is -1; adding the sentinel gives a total
//   that is too small
// - BFS from dest gives dist(m, dest) only because roads are two-way; one-way roads
//   need that BFS on the reversed graph
// - best starts at -1, so test best == -1 before comparing totals
// - dist[start] = 0 before the loop and mark on push, as in every BFS
// Tests to write: meet midway; meet at dest; b lies on a's route; a == b; no common
//   city -> -1.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;
using std::greater;

// dist[city] = roads from start, -1 if unreachable. T: O(V + E), S: O(V).
vector<int> bfs(const vector<vector<int>>& graph, int start) {
    vector<int> dist(graph.size(), -1);
    dist[start] = 0;
    queue<int> q;
    q.push(start);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nxt : graph[cur]) {
            if (dist[nxt] != -1) { continue; }
            dist[nxt] = dist[cur] + 1;
            q.push(nxt);
        }
    }
    return dist;
}

// three BFS (from a, from b, from dest; undirected, so dest needs no reversed graph),
// then try every meeting city m. T: O(V + E), S: O(V + E).
int sharedRouteDistance(int n, const vector<vector<int>>& edges, int a, int b, int dest) {
    // graph[u] = cities one road away from u
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }
    vector<int> fromA = bfs(graph, a);
    vector<int> fromB = bfs(graph, b);
    vector<int> toDest = bfs(graph, dest);

    int best = -1;
    for (int m = 0; m < n; m++) {
        if (fromA[m] == -1 || fromB[m] == -1 || toDest[m] == -1) { continue; }
        int total = fromA[m] + fromB[m] + toDest[m];
        if (best == -1 || total < best) { best = total; }
    }
    return best;
}

// dist[v] = cheapest cost from start to v, LLONG_MAX if unreachable.
// T: O((V + E) log V), S: O(V).
vector<long long> dijkstra(const vector<vector<pair<int, int>>>& graph, int start) {
    vector<long long> dist(graph.size(), LLONG_MAX);
    // (cost so far, node), cheapest on top
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> heap;
    dist[start] = 0;
    heap.push({0, start});
    while (!heap.empty()) {
        auto [cost, cur] = heap.top();
        heap.pop();
        // stale entry: cur was already settled cheaper
        if (cost > dist[cur]) { continue; }
        for (auto [nxt, w] : graph[cur]) {
            if (cost + w >= dist[nxt]) { continue; }
            dist[nxt] = cost + w;
            heap.push({dist[nxt], nxt});
        }
    }
    return dist;
}

// follow-up (LC 2203): one-way roads [u, v, w], w >= 0. Dijkstra from a and from b on the
// graph, and from dest on the reversed graph, which gives dist(m, dest); then the same
// min over m. Pitfall: test LLONG_MAX before adding; sums need long long.
// T: O((V + E) log V), S: O(V + E).
long long sharedRouteWeighted(int n, const vector<vector<int>>& edges, int a, int b,
                              int dest) {
    // graph[u] = (v, w) per road u -> v; reversed[v] = (u, w) per road u -> v
    vector<vector<pair<int, int>>> graph(n);
    vector<vector<pair<int, int>>> reversed(n);
    for (const vector<int>& edge : edges) {
        graph[edge[0]].push_back({edge[1], edge[2]});
        reversed[edge[1]].push_back({edge[0], edge[2]});
    }
    vector<long long> fromA = dijkstra(graph, a);
    vector<long long> fromB = dijkstra(graph, b);
    vector<long long> toDest = dijkstra(reversed, dest);

    long long best = -1;
    for (int m = 0; m < n; m++) {
        if (fromA[m] == LLONG_MAX || fromB[m] == LLONG_MAX || toDest[m] == LLONG_MAX) {
            continue;
        }
        long long total = fromA[m] + fromB[m] + toDest[m];
        if (best == -1 || total < best) { best = total; }
    }
    return best;
}

void test(const string& label, int n, const vector<vector<int>>& edges, int a, int b,
          int dest, int want) {
    bool ok = sharedRouteDistance(n, edges, a, b, dest) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testWeighted(const string& label, int n, const vector<vector<int>>& edges, int a,
                  int b, int dest, long long want) {
    bool ok = sharedRouteWeighted(n, edges, a, b, dest) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("meet-midway", 4, {{0, 2}, {1, 2}, {2, 3}}, 0, 1, 3, 3);
    test("meet-at-dest", 3, {{0, 2}, {1, 2}}, 0, 1, 2, 2);
    test("pick-up", 3, {{0, 1}, {1, 2}}, 0, 1, 2, 2);
    test("same-start", 3, {{0, 1}, {1, 2}}, 0, 0, 2, 2);
    test("cut-off", 4, {{0, 1}, {2, 3}}, 0, 1, 3, -1);
    testWeighted("w-merge-early", 4,
                 {{0, 2, 1}, {1, 2, 1}, {2, 3, 5}, {0, 3, 6}, {1, 3, 6}}, 0, 1, 3, 7);
    testWeighted("w-reversed", 4, {{0, 2, 2}, {1, 2, 3}, {2, 3, 4}, {3, 2, 1}},
                 0, 1, 3, 9);
    testWeighted("w-one-way-cut", 3, {{0, 2, 1}, {2, 1, 1}}, 0, 1, 2, -1);
    testWeighted("w-big-weights", 3, {{0, 2, 1000000000}, {1, 2, 1000000000}}, 0, 1, 2,
                 2000000000LL);
}

// follow-up: weighted one-way roads (LC 2203) -> sharedRouteWeighted above; two-way
//   weighted roads: add both directions to graph and run dest on graph itself
// follow-up: one-way roads, unweighted -> keep bfs, but build a reversed graph and run
//   the dest BFS on it, as sharedRouteWeighted does
// follow-up: also return the meeting city -> keep the m that set best:
//   if (best == -1 || total < best) { best = total; meet = m; }
//   the routes then come from a parent[] per BFS, walked back from meet
