// 48 · Minimum Time To Collect All Apples In A Tree (Cisco LC tag, Medium) — LC 1443
// undirected tree of n nodes rooted at 0; walking an edge takes 1 s; start and end at
// node 0, collect every apple; return the min time
//
// clarify:
// edges -> n - 1 undirected, given in any direction
// no apples -> 0
// apple at node 0 -> costs nothing
// n up to 1e5 -> a chain is 1e5 deep, so no recursion

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: an edge (parent, v) is walked twice iff v's subtree has an apple; BFS from 0
// gives parents and an order, then process nodes leaves-first to push "needed" upward
// T: O(V), S: O(V)
int minTime(int n, const vector<vector<int>>& edges, const vector<bool>& hasApple) {
    // graph[u] = neighbors of u
    vector<vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    vector<int> parent(n, -1);
    // nodes in BFS order from the root
    vector<int> order;
    vector<bool> seen(n, false);
    order.push_back(0);
    seen[0] = true;
    for (int k = 0; k < (int)order.size(); k++) {
        int cur = order[k];
        for (int nxt : graph[cur]) {
            if (seen[nxt]) { continue; }
            seen[nxt] = true;
            parent[nxt] = cur;
            order.push_back(nxt);
        }
    }
    // need[v] = v's subtree holds an apple
    vector<bool> need = hasApple;
    int time = 0;
    // reverse BFS order: every child is done before its parent
    for (int k = n - 1; k >= 1; k--) {
        int v = order[k];
        if (need[v]) {
            time += 2;
            need[parent[v]] = true;
        }
    }
    return time;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<bool>& hasApple, int want) {
    cout << label << (minTime(n, edges, hasApple) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> tree = {{0, 1}, {0, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 6}};
    test("three-apples", 7, tree, {false, false, true, false, true, true, false}, 8);
    test("two-apples", 7, tree, {false, false, true, false, false, true, false}, 6);
    test("no-apples", 7, tree, vector<bool>(7, false), 0);
    test("reversed-edge", 2, {{1, 0}}, {false, true}, 2);
    return 0;
}

// follow-up:
// 1. recursive -> dfs(v, parent) adds child time + 2 for children with apples below
// 2. within distance 2 (LC 2603) -> trim coinless leaves, then 2 layers of leaves
// 3. no need to return to 0 -> 2 * needed edges - depth of the deepest needed node
