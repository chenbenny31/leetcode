// 55 · Sum of Distances in Tree — LC 834
// n nodes, n - 1 undirected edges forming a tree
// (1) LC 834: ans[i] = sum of distances (edge cnt) from node i to every other node
// (2) weighted edges {u, v, w}: distance = sum of w on the path
// reported: NG onsite x2, (2) is the reported version
//
// clarify:
// n up to 3e4 -> BFS per node, O(n^2), too slow
// depth up to n -> no recursion, iterate in BFS order
// (1) sums -> fit in int
// (2) sums -> long long; w >= 0 not needed

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// (1) unweighted (LC 834)
// core idea: root at 0; bottom-up get size[v] and ans[0] = sum of depths, then reroot
// root parent -> child: size[child] nodes 1 closer, n - size[child] nodes 1 farther
// T: O(V + E), S: O(V)
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

// (2) weighted edges
// core idea: same 2 passes; crossing edge parent -> child of weight w:
// size[child] nodes get w closer, the other n - size[child] get w farther
// T: O(V + E), S: O(V)
vector<long long> sumOfDistancesWeighted(int n, const vector<vector<int>>& edges) {
    // graph[v] = (neighbor, edge weight)
    vector<vector<pair<int, int>>> graph(n);
    for (const vector<int>& e : edges) {
        graph[e[0]].push_back({e[1], e[2]});
        graph[e[1]].push_back({e[0], e[2]});
    }
    vector<int> parent(n, -1);
    // upWeight[v] = weight of the edge v - parent[v]; dist[v] = path weight from 0
    vector<long long> upWeight(n, 0);
    vector<long long> dist(n, 0);
    vector<int> order = {0};
    for (size_t i = 0; i < order.size(); i++) {
        int cur = order[i];
        for (const auto& [nxt, w] : graph[cur]) {
            if (nxt == parent[cur]) { continue; }
            parent[nxt] = cur;
            upWeight[nxt] = w;
            dist[nxt] = dist[cur] + w;
            order.push_back(nxt);
        }
    }
    vector<long long> size(n, 1);
    vector<long long> ans(n, 0);
    for (int i = n - 1; i > 0; i--) {
        int v = order[i];
        size[parent[v]] += size[v];
        ans[0] += dist[v];
    }
    for (int i = 1; i < n; i++) {
        int v = order[i];
        ans[v] = ans[parent[v]] + upWeight[v] * (n - 2 * size[v]);
    }
    return ans;
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const vector<int>& want) {
    cout << label << (sumOfDistancesInTree(n, edges) == want ? " PASS\n" : " FAIL\n");
}

void testWeighted(const string& label, int n, const vector<vector<int>>& edges,
                  const vector<long long>& want) {
    cout << label << (sumOfDistancesWeighted(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", 1, {}, {0});
    test("two", 2, {{0, 1}}, {1, 1});
    test("star", 4, {{0, 1}, {0, 2}, {0, 3}}, {3, 5, 5, 5});
    test("path", 3, {{0, 1}, {1, 2}}, {3, 2, 3});

    testWeighted("w-single", 1, {}, {0});
    testWeighted("w-path", 3, {{0, 1, 2}, {1, 2, 3}}, {7, 5, 8});
    testWeighted("w-star", 4, {{0, 1, 1}, {0, 2, 2}, {0, 3, 3}}, {6, 8, 10, 12});
    testWeighted("w-root-is-leaf", 4, {{0, 1, 5}, {1, 2, 1}, {1, 3, 1}}, {17, 7, 9, 9});
}

// follow-up:
// why not BFS from every node -> O(n^2); rerooting reuses the parent's answer
// node w/ smallest sum -> argmin of ans (tree median / centroid)
// sum over only marked nodes -> size[v] = marked nodes in the subtree, n = total marked
// max distance instead of sum (LC 310 style) -> rerooting w/ top-2 child heights
// recursive version -> 2 DFS (post-order sizes, pre-order reroot); depth may overflow
