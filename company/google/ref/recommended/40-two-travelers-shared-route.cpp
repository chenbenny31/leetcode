// Two Travelers Shared Route (Google, interview report; LC 2203 style). n cities 0..n-1,
// undirected unweighted roads (assumed; weighted -> follow-up). Travelers start at a and
// b and must reach dest; they may meet at a city m and ride together from m to dest, the
// shared part counted once. Return min dist(a, m) + dist(b, m) + dist(m, dest) over all
// m (m may be a, b or dest itself), or -1 if no m works.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

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

void test(const string& label, int n, const vector<vector<int>>& edges, int a, int b,
          int dest, int want) {
    bool ok = sharedRouteDistance(n, edges, a, b, dest) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("meet-midway", 4, {{0, 2}, {1, 2}, {2, 3}}, 0, 1, 3, 3);
    test("meet-at-dest", 3, {{0, 2}, {1, 2}}, 0, 1, 2, 2);
    test("pick-up", 3, {{0, 1}, {1, 2}}, 0, 1, 2, 2);
    test("same-start", 3, {{0, 1}, {1, 2}}, 0, 0, 2, 2);
    test("cut-off", 4, {{0, 1}, {2, 3}}, 0, 1, 3, -1);
}

// follow-up: weighted roads (LC 2203) -> same plan with three Dijkstra runs, long long
//   distances (sum can overflow int), LLONG_MAX as "unreachable"; T: O((V + E) log V)
// follow-up: one-way roads -> BFS / Dijkstra from a and b on the graph, from dest on the
//   reversed graph (edge v -> u for every u -> v) to get dist(m, dest)
// follow-up: also return the meeting city -> keep the m that set best
