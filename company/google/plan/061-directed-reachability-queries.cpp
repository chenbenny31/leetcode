// Directed Reachability Queries (Google, interview report)
// (1) n nodes, edges [u, v] = u -> v; query {source, target}: can source reach target?
// (2) big cycles: condense SCCs first
//
// clarify:
// edges -> one-way
// source == target w/o self-loop -> true
// cycles, dup edges, self-loops -> all ok
// V, E, num of queries -> pick the method

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// graph[u] = nodes v w/ an edge u -> v
vector<vector<int>> buildGraph(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) { graph[edge[0]].push_back(edge[1]); }
    return graph;
}

// BFS; seen[v] = source reaches v
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

// (1) solution 1: BFS per query; use when queries are few
// core idea: one BFS per query
// T: O(q (V + E)), S: O(V + E)
vector<bool> canReachNaive(int n, const vector<vector<int>>& edges,
                           const vector<vector<int>>& queries) {
    vector<vector<int>> graph = buildGraph(n, edges);
    vector<bool> result;
    for (const vector<int>& query : queries) {
        result.push_back(reachableFrom(graph, query[0])[query[1]]);
    }
    return result;
}

// (1) solution 2: precompute all rows; use when queries are many
// core idea: BFS once from every node, keep each node's row; a query is a lookup
// T: O(V (V + E) + q), S: O(V^2)
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

// (2) SCC condensation
// core idea: Kosaraju; comp ids come out in topological order -> fill rows last id first
// reach[c] = {c} OR the rows of its successors
// T: O(V + E + C * E'), S: O(V + E + C^2) — C = components, E' = edges between them
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
    // dag[c] = components w/ an edge from c (dups are harmless)
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
    test("two-cycles", 5, {{0, 1}, {1, 0}, {1, 2}, {2, 3}, {3, 2}, {4, 3}},
         {{0, 3}, {3, 0}, {4, 2}, {2, 4}, {1, 1}}, {true, false, true, false, true});
    test("reverse-ids", 4, {{3, 2}, {2, 1}, {1, 0}}, {{3, 0}, {0, 3}, {2, 1}},
         {true, false, true});
}

// follow-up:
// dense graph -> transitive closure on bitset rows (Floyd-Warshall style), O(V^3 / 64)
// edges only added online -> every x reaching u ORs in v's row, O(V^2 / 64) per edge
// edges also removed -> no cheap idx: BFS per query, or bidirectional BFS
// return a path -> parent[nxt] = cur in the BFS, walk back from target
