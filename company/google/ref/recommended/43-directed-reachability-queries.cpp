// Directed Reachability Queries (Google, interview report). n nodes 0..n-1, directed
// edges [u, v] (u -> v). For each query {source, target} answer whether source can reach
// target along edges. A node always reaches itself.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// graph[u] = nodes v with an edge u -> v
vector<vector<int>> buildGraph(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) { graph[edge[0]].push_back(edge[1]); }
    return graph;
}

// seen[v] = source reaches v. T: O(V + E), S: O(V).
vector<bool> reachableFrom(const vector<vector<int>>& graph, int source) {
    vector<bool> seen(graph.size(), false);
    seen[source] = true;
    queue<int> q;
    q.push(source);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int nxt : graph[cur]) {
            if (seen[nxt]) { continue; }
            seen[nxt] = true;
            q.push(nxt);
        }
    }
    return seen;
}

// straightforward: one BFS per query. T: O(q (V + E)), S: O(V + E).
vector<bool> canReachNaive(int n, const vector<vector<int>>& edges,
                           const vector<vector<int>>& queries) {
    vector<vector<int>> graph = buildGraph(n, edges);
    vector<bool> result;
    for (const vector<int>& query : queries) {
        result.push_back(reachableFrom(graph, query[0])[query[1]]);
    }
    return result;
}

// improved for many queries: BFS once from every node, then O(1) per query.
// T: O(V (V + E) + q), S: O(V^2) bits.
vector<bool> canReachPrecomputed(int n, const vector<vector<int>>& edges,
                                 const vector<vector<int>>& queries) {
    vector<vector<int>> graph = buildGraph(n, edges);
    // reach[u][v] = u reaches v
    vector<vector<bool>> reach;
    for (int u = 0; u < n; u++) { reach.push_back(reachableFrom(graph, u)); }
    vector<bool> result;
    for (const vector<int>& query : queries) {
        result.push_back(reach[query[0]][query[1]]);
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<vector<int>>& queries, const vector<bool>& want) {
    bool ok = canReachNaive(n, edges, queries) == want;
    ok = ok && canReachPrecomputed(n, edges, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}},
         {{0, 3}, {3, 0}, {1, 2}, {2, 2}}, {true, false, false, true});
    test("cycle", 4, {{0, 1}, {1, 2}, {2, 0}}, {{2, 1}, {0, 3}, {3, 3}},
         {true, false, true});
    test("no-edges", 2, {}, {{0, 0}, {0, 1}}, {true, false});
}

// follow-up: big cycles -> condense SCCs first (Tarjan / Kosaraju): nodes in one SCC
//   share an answer, so precompute on the smaller DAG; in reverse topological order
//   reach[c] = {c} OR the reach of every successor (bitset OR, O(C * E' / 64))
// follow-up: dense graph -> Floyd-Warshall transitive closure with bitset rows:
//   if reach[i][k] then reach[i] |= reach[k]; O(V^3 / 64)
// follow-up: queries online, graph changes -> no cheap index; fall back to BFS per query
//   (or bidirectional BFS: from source forward and from target on the reversed graph)
