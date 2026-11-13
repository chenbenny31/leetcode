// Two Travelers Shared Route (Google, interview report; LC 2203 style).
// (1) unweighted two-way roads; min dist(a, m) + dist(b, m) + dist(m, dest) over m, or -1
// (2) LC 2203: one-way roads {u, v, w}, w >= 0; same min
//
// clarify:
// meet city m -> shared part m -> dest counted once; m may be a, b or dest
// a == b -> allowed
// V, E up to 1e5 -> no BFS per m
// parallel roads, self-loops -> harmless
// return -> total len only; meeting city in follow-up

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

// dist[city] = roads from start, -1 if unreachable; O(V + E)
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

// (1) unweighted, two-way
// core idea: routes never split after meeting -> a "Y": a -> m, b -> m, m -> dest
// BFS from a, b, dest (two-way, so no reversed graph); min sum over m
// T: O(V + E), S: O(V + E)
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
        // adding the -1 sentinel would give a too-small total
        if (fromA[m] == -1 || fromB[m] == -1 || toDest[m] == -1) { continue; }
        int total = fromA[m] + fromB[m] + toDest[m];
        if (best == -1 || total < best) { best = total; }
    }
    return best;
}

// dist[v] = cheapest cost from start to v, LLONG_MAX if unreachable; O((V + E) log V)
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
        // stale entry: cur already settled cheaper
        if (cost > dist[cur]) { continue; }
        for (auto [nxt, w] : graph[cur]) {
            if (cost + w >= dist[nxt]) { continue; }
            dist[nxt] = cost + w;
            heap.push({dist[nxt], nxt});
        }
    }
    return dist;
}

// (2) weighted, one-way (LC 2203)
// core idea: Dijkstra from a and b on graph, from dest on reversed graph = dist(m, dest)
// T: O((V + E) log V), S: O(V + E)
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
    test("cut-off", 4, {{0, 1}, {2, 3}}, 0, 1, 3, -1);
    testWeighted("w-merge-early", 4,
                 {{0, 2, 1}, {1, 2, 1}, {2, 3, 5}, {0, 3, 6}, {1, 3, 6}}, 0, 1, 3, 7);
    testWeighted("w-reversed", 4, {{0, 2, 2}, {1, 2, 3}, {2, 3, 4}, {3, 2, 1}},
                 0, 1, 3, 9);
    testWeighted("w-one-way-cut", 3, {{0, 2, 1}, {2, 1, 1}}, 0, 1, 2, -1);
    testWeighted("w-big-weights", 3, {{0, 2, 1000000000}, {1, 2, 1000000000}}, 0, 1, 2,
                 2000000000LL);
}

// follow-up:
// two-way weighted roads -> add both directions, run dest Dijkstra on graph itself
// one-way unweighted roads -> keep bfs, run dest BFS on a reversed graph
// also return meeting city -> keep the m that set best; routes from a parent[] per BFS
// three travelers -> one more search if all merge at one city; else Steiner tree
