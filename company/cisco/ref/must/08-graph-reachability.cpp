// 08 · Reachable nodes — graph basics drill (2026-11-27)
// copy of `select/08-graphs/10-graph-reachability.cpp`
// Reachable Nodes (graph basics) — hash-map adjacency list + DFS from start
// edges are directed (u -> v); node ids are any ints, negative too
// return every node reachable from start, in any order
//
// clarify:
// start itself -> counts as reachable (ask first: it decides every expected value)
// start in no edge -> still returns {start} (ask; the other reading returns {})
// cycles, self loops, repeated edges -> allowed; each node is returned once
// node ids -> any int, not 0 .. n - 1, so the graph is a hash map, not a vector
// order -> any, so the test sorts both sides before it compares

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::sort;

struct Edge {
    int u;
    int v;
};

// core idea: adjacency list keyed by node id, then a DFS with an explicit stack from
// start; a node is marked when it is pushed, so it is pushed once
// T: O(V + E), S: O(V + E)
vector<int> reachable(const vector<Edge>& edges, int start) {
    unordered_map<int, vector<int>> graph; // u -> nodes v with an edge u -> v
    for (const Edge& edge : edges) { graph[edge.u].push_back(edge.v); }

    vector<int> result;
    unordered_set<int> seen = {start};
    vector<int> pending = {start}; // nodes seen but not expanded yet
    while (!pending.empty()) {
        int cur = pending.back();
        pending.pop_back();
        result.push_back(cur);
        // find, not []: a node with no out-edges has no entry, and that is fine
        auto it = graph.find(cur);
        if (it == graph.end()) { continue; }
        for (int nxt : it->second) {
            if (seen.count(nxt)) { continue; }
            seen.insert(nxt);
            pending.push_back(nxt);
        }
    }
    return result;
}

void test(const string& label, const vector<Edge>& edges, int start, vector<int> want) {
    vector<int> got = reachable(edges, start);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain-from-head", {{1, 2}, {2, 3}}, 1, {1, 2, 3});
    test("sink-only-itself", {{1, 2}}, 2, {2});
    test("cycle-and-negative-id", {{1, -2}, {-2, 1}, {5, 1}}, 1, {1, -2});
    test("start-in-no-edge", {{1, 2}}, 9, {9});
}

// follow-up:
// 1. recursive DFS -> shorter, but a long chain can overflow the call stack
// 2. hop count to each node -> BFS with a queue, dist[nxt] = dist[cur] + 1
// 3. nodes that can reach start -> build the reversed graph, same search
// 4. start in no edge must return {} -> track whether start appears while building
// 5. undirected edges -> push both directions; then union-find works too
// 6. ids are 0 .. n - 1 -> vector<vector<int>> graph and vector<bool> seen
// 7. is there a path start -> target -> same search, return true when cur == target
