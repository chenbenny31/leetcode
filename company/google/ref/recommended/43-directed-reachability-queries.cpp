// Directed Reachability Queries (Google, interview report). n nodes 0..n-1, directed
// edges [u, v] (u -> v). For each query {source, target} answer whether source can reach
// target along edges. A node always reaches itself.
//
// Clarify:
// - edges are one-way: u -> v does not give v -> u
// - source == target -> true even without a self-loop (stated)
// - cycles, duplicate edges, self-loops -> allowed; seen[] handles them
// - sizes pick the method: few queries -> a BFS per query, O(q (V + E)); many queries
//   on a fixed graph with V up to a few thousand -> precompute reach[u][v] (V^2 bits)
// - return one bool per query, in query order
// Idea: one BFS from source answers one query. With many queries, BFS once from every
//   node and keep each node's row, so a query is a lookup. With big cycles every node
//   of one SCC has the same row, so condense first (follow-up function).
// Pitfalls:
// - build only graph[u].push_back(v); adding v -> u too answers a different question
// - seen[source] = true before the loop, so source reaches itself and is queued once
// - in the precomputed version build the rows once, outside the query loop
// - V x V bools for V = 1e5 is 1e10 bits: condense, or fall back to a BFS per query
// Tests to write: diamond (a target via two paths, no way back, siblings, self); a
//   cycle plus an outside node; no edges; two cycles joined one way; ids in reverse
//   topological order.

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

// post-order DFS: appends cur to order after all its unseen descendants
void dfsFinish(int cur, const vector<vector<int>>& graph, vector<bool>& seen,
               vector<int>& order) {
    seen[cur] = true;
    for (int nxt : graph[cur]) {
        if (!seen[nxt]) { dfsFinish(nxt, graph, seen, order); }
    }
    order.push_back(cur);
}

// gives component id to cur and to every unlabeled node that reaches it
void dfsLabel(int cur, const vector<vector<int>>& reversed, int id, vector<int>& comp) {
    comp[cur] = id;
    for (int nxt : reversed[cur]) {
        if (comp[nxt] == -1) { dfsLabel(nxt, reversed, id, comp); }
    }
}

// follow-up (big cycles): Kosaraju condenses the SCCs. Pass 1 records DFS finish order
// on the graph; pass 2 takes nodes latest-finish first and labels on the reversed graph.
// Ids then follow topological order (an edge between components goes from a lower id
// to a higher one), so rows fill from the last id back: reach[c] = {c} OR the rows of
// its successors. Pitfall: pass 2 needs the reversed graph AND decreasing finish time;
// both DFS recurse up to V deep (use an explicit stack for V ~ 1e5).
// T: O(V + E + C * E'), with bitset rows / 64; S: O(V + E + C^2); C = components,
// E' = edges between components.
vector<bool> canReachCondensed(int n, const vector<vector<int>>& edges,
                               const vector<vector<int>>& queries) {
    vector<vector<int>> graph = buildGraph(n, edges);
    // reversed[v] = nodes u with an edge u -> v
    vector<vector<int>> reversed(n);
    for (const vector<int>& edge : edges) { reversed[edge[1]].push_back(edge[0]); }
    vector<bool> seen(n, false);
    // nodes in increasing DFS finish time
    vector<int> order;
    for (int u = 0; u < n; u++) {
        if (!seen[u]) { dfsFinish(u, graph, seen, order); }
    }
    // comp[u] = SCC id of u, -1 until labeled
    vector<int> comp(n, -1);
    int components = 0;
    for (int i = n - 1; i >= 0; i--) {
        if (comp[order[i]] != -1) { continue; }
        dfsLabel(order[i], reversed, components, comp);
        components++;
    }
    // dag[c] = components with an edge from c (duplicates are harmless)
    vector<vector<int>> dag(components);
    for (const vector<int>& edge : edges) {
        int from = comp[edge[0]];
        int to = comp[edge[1]];
        if (from != to) { dag[from].push_back(to); }
    }
    // reach[c][d] = component c reaches component d
    vector<vector<bool>> reach(components, vector<bool>(components, false));
    for (int c = components - 1; c >= 0; c--) {
        reach[c][c] = true;
        for (int d : dag[c]) {
            // d reaches only ids >= d, and its row is complete since d > c
            for (int x = d; x < components; x++) {
                if (reach[d][x]) { reach[c][x] = true; }
            }
        }
    }
    vector<bool> result;
    for (const vector<int>& query : queries) {
        result.push_back(reach[comp[query[0]]][comp[query[1]]]);
    }
    return result;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<vector<int>>& queries, const vector<bool>& want) {
    bool ok = canReachNaive(n, edges, queries) == want;
    ok = ok && canReachPrecomputed(n, edges, queries) == want;
    ok = ok && canReachCondensed(n, edges, queries) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}},
         {{0, 3}, {3, 0}, {1, 2}, {2, 2}}, {true, false, false, true});
    test("cycle", 4, {{0, 1}, {1, 2}, {2, 0}}, {{2, 1}, {0, 3}, {3, 3}},
         {true, false, true});
    test("no-edges", 2, {}, {{0, 0}, {0, 1}}, {true, false});
    test("two-cycles", 5, {{0, 1}, {1, 0}, {1, 2}, {2, 3}, {3, 2}, {4, 3}},
         {{0, 3}, {3, 0}, {4, 2}, {2, 4}, {1, 1}}, {true, false, true, false, true});
    test("reverse-ids", 4, {{3, 2}, {2, 1}, {1, 0}}, {{3, 0}, {0, 3}, {2, 1}},
         {true, false, true});
}

// follow-up: big cycles -> canReachCondensed above (Kosaraju + rows per component)
// follow-up: dense graph -> transitive closure, Floyd-Warshall style on bitset<N> rows
//   (N a compile-time bound), O(V^3 / 64):
//   for (int k = 0; k < n; k++) {
//       for (int i = 0; i < n; i++) {
//           if (reach[i][k]) { reach[i] |= reach[k]; }
//       }
//   }
// follow-up: edges only added online -> keep the rows; a new edge u -> v gives every x
//   that reaches u all of v's row, O(V^2 / 64) per edge with bitset rows:
//   for (int x = 0; x < n; x++) {
//       if (reach[x][u]) { reach[x] |= reach[v]; }
//   }
// follow-up: edges also removed -> no cheap index; BFS per query (or bidirectional BFS:
//   from source forward and from target on the reversed graph, stop when they meet)
// follow-up: return a path -> in reachableFrom set parent[nxt] = cur when marking, then
//   for (int v = target; v != source; v = parent[v]) { path.push_back(v); }
//   path.push_back(source);  reverse(path.begin(), path.end());
