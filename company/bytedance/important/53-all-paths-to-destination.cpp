// ByteDance (1p3a infra onsite, round 3) / LC 1059: a directed graph has n nodes and
// edges[i] = [a, b] (self-loops and repeated edges allowed). Given source and
// destination, return true iff every path starting at source ends at destination:
// at least one path exists, every node reachable from source that has no outgoing
// edge is destination, and no path can go on forever (no reachable cycle).
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// color[u]: 0 = not visited, 1 = on the current DFS path, 2 = done and every path from
// u ends at destination.
// Reaching a node that is on the current path means a cycle; a node with no outgoing
// edge is fine only if it is destination.
bool dfs(const vector<vector<int>>& graph, int u, int destination, vector<int>& color) {
    if (color[u] == 1) {
        return false;
    }
    if (color[u] == 2) {
        return true;
    }
    if (graph[u].empty()) {
        return u == destination;
    }

    color[u] = 1;
    for (int nxt : graph[u]) {
        if (!dfs(graph, nxt, destination, color)) {
            return false;
        }
    }
    color[u] = 2;
    return true;
}

// DFS from source with three colors. T: O(V + E), S: O(V + E).
bool leadsToDestination(int n, const vector<vector<int>>& edges, int source,
                        int destination) {
    // graph[u] = nodes u has an edge to
    vector<vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
    }
    vector<int> color(n, 0);
    return dfs(graph, source, destination, color);
}

void test(const string& label, int n, const vector<vector<int>>& edges, int source,
          int destination, bool want) {
    bool got = leadsToDestination(n, edges, source, destination);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, 0, 3, true);
    test("dead-end", 3, {{0, 1}, {0, 2}}, 0, 2, false);
    test("cycle", 3, {{0, 1}, {1, 2}, {2, 1}}, 0, 2, false);
    test("dest-has-exit", 3, {{0, 1}, {1, 2}}, 0, 1, false);
    test("self-loop", 2, {{0, 1}, {1, 1}}, 0, 1, false);
    test("same-node", 1, {}, 0, 0, true);
}
