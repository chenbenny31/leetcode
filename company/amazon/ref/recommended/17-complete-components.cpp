// LC 2685: an undirected graph has nodes 0..n-1 and no repeated edges. Return how many
// connected components are complete (every pair of their nodes is joined by an edge).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// count the nodes of cur's component and the sum of their degrees
void dfs(int cur, const vector<vector<int>>& graph, vector<bool>& seen, int& nodes,
         int& degrees) {
    seen[cur] = true;
    nodes++;
    degrees += graph[cur].size();
    for (int nxt : graph[cur]) {
        if (!seen[nxt]) { dfs(nxt, graph, seen, nodes, degrees); }
    }
}

// A component with v nodes is complete iff it has v(v-1)/2 edges, i.e. its degree sum is
// v(v-1). T: O(V + E), S: O(V + E).
int countCompleteComponents(int n, const vector<vector<int>>& edges) {
    // graph[u] = neighbors of u
    vector<vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    vector<bool> seen(n, false);
    int complete = 0;
    for (int u = 0; u < n; u++) {
        if (seen[u]) { continue; }
        int nodes = 0, degrees = 0;
        dfs(u, graph, seen, nodes, degrees);
        if (degrees == nodes * (nodes - 1)) { complete++; }
    }
    return complete;
}

void test(const string& label, int n, const vector<vector<int>>& edges, int want) {
    cout << label << (countCompleteComponents(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("triangle", 3, {{0, 1}, {1, 2}, {0, 2}}, 1);
    test("path", 3, {{0, 1}, {1, 2}}, 0);
    test("singles", 2, {}, 2);
    test("mixed", 5, {{0, 1}, {2, 3}, {3, 4}}, 1);
}
