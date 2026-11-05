// ByteDance (1p3a infra onsite, round 3) / LC 1059: directed graph, edges[i] = [a, b]
// (self-loops allowed). Return true iff every path from source ends at destination:
// every reachable node with no outgoing edge is destination, and no reachable cycle.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// true iff every path from cur ends at destination
bool dfs(const vector<vector<int>>& graph, int cur, int destination, vector<int>& color) {
    // cur is on the current path: a cycle
    if (color[cur] == 1) {
        return false;
    }
    if (color[cur] == 2) {
        return true;
    }
    if (graph[cur].empty()) {
        return cur == destination;
    }

    color[cur] = 1;
    for (int nxt : graph[cur]) {
        if (!dfs(graph, nxt, destination, color)) {
            return false;
        }
    }
    color[cur] = 2;
    return true;
}

// DFS from source with three colors. T: O(V + E), S: O(V + E).
bool leadsToDestination(int n, const vector<vector<int>>& edges, int source,
                        int destination) {
    // graph[u] = nodes u has an edge to
    vector<vector<int>> graph(n);
    for (const vector<int>& e : edges) {
        graph[e[0]].push_back(e[1]);
    }
    // color[u]: 0 = unvisited, 1 = on the current DFS path, 2 = done and good
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
    test("same-node", 1, {}, 0, 0, true);
}

// follow-up: deep graph, recursion limit -> iterative DFS, stack of (node, next edge
//   idx) with the same 3 colors
// follow-up: eventual safe states (LC 802) -> same 3-color DFS from every node; any
//   sink is good, safe iff color 2
// follow-up: count paths source -> destination -> after the check the reachable part is
//   a DAG; memo ways[u] = sum ways[nxt], ways[destination] = 1; O(V + E)
