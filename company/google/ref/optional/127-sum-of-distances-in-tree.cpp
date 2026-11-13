// LC 834 Sum of Distances in Tree: n nodes, n - 1 undirected edges forming a connected
// tree. Return ans where ans[i] = sum of distances from node i to every other node.
//
// Clarify:
// - n up to 3e4 -> a BFS from every node (O(n^2)) is too slow
// - depth up to n -> iterative order (BFS, then reverse) instead of recursion
// - sums <= n^2 / 2 = 4.5e8 -> fits in int
// Idea: root at 0. Pass 1 (children before parents): size[v] = nodes in v's subtree;
//   ans[0] = sum of depths. Pass 2 (parents first): moving the root from parent to
//   child brings size[child] nodes 1 closer and n - size[child] nodes 1 farther:
//   ans[child] = ans[parent] - size[child] + (n - size[child]).
// Pitfalls:
// - pass 1 in reverse BFS order, pass 2 in BFS order
// - skip the parent when walking neighbors
// Tests to write: single node; two nodes; star; path.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// BFS order from 0, sizes bottom-up, then reroot top-down. T: O(V + E), S: O(V).
vector<int> sumOfDistancesInTree(int n, const vector<vector<int>>& edges) {
    // graph[v] = tree neighbors of v (undirected edges)
    vector<vector<int>> graph(n);
    for (const vector<int>& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    // parent[v] in the tree rooted at 0; depth[v] = distance from 0
    vector<int> parent(n, -1);
    vector<int> depth(n, 0);
    // order = nodes in BFS order, every parent before its children
    vector<int> order = {0};
    for (size_t i = 0; i < order.size(); i++) {
        int cur = order[i];
        for (int nxt : graph[cur]) {
            if (nxt == parent[cur]) { continue; }
            parent[nxt] = cur;
            depth[nxt] = depth[cur] + 1;
            order.push_back(nxt);
        }
    }
    // size[v] = nodes in v's subtree
    vector<int> size(n, 1);
    vector<int> ans(n, 0);
    for (int i = n - 1; i > 0; i--) {
        int v = order[i];
        size[parent[v]] += size[v];
        ans[0] += depth[v];
    }
    for (int i = 1; i < n; i++) {
        int v = order[i];
        ans[v] = ans[parent[v]] - size[v] + (n - size[v]);
    }
    return ans;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<int>& want) {
    cout << label << (sumOfDistancesInTree(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", 1, {}, {0});
    test("two", 2, {{0, 1}}, {1, 1});
    test("star", 4, {{0, 1}, {0, 2}, {0, 3}}, {3, 5, 5, 5});
    test("path", 3, {{0, 1}, {1, 2}}, {3, 2, 3});
}

// follow-up: weighted edges -> pass 1 sums w * size[child] per edge for ans[0];
//     ans[child] = ans[parent] + w * (n - 2 * size[child]);
// follow-up: node with the smallest sum -> argmin of ans (a tree median / centroid)
