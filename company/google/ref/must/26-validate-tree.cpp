// Validate Tree (Google, interview report)
// (1) parent[i] = parent of i, -1 = root, no self-loops; valid iff 1 root reaches all
// (2) n nodes, undirected edges: min root making a binary tree (<= 2 children), or -1
// (3) + colors[i] = 'B' / 'W': all roots (ascending) where parent / child colors differ
// (4) (J30) colors any char: smallest root w/ every depth a single color, or -1
//
// clarify:
// empty input -> (1) false: no root
// dup edges / self-loops -> ask; assumed none
// > 2 children in (1) -> allowed; binary check is a follow-up
// several valid roots -> smallest in (2) / (4), all in (3)
// n = 1, no edges -> root 0

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// (1) parent array -> valid tree?
// core idea: build child lists, BFS from the single root, count reached nodes
// a node on a cycle never connects to the root, so it stays unreached
// T: O(n), S: O(n)
bool validTreeFromParent(const vector<int>& parent) {
    int n = parent.size();
    // graph[p] = children of p (edge parent -> child)
    vector<vector<int>> graph(n);
    int root = -1;
    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) {
            graph[parent[i]].push_back(i);
            continue;
        }
        if (root != -1) { return false; }
        root = i;
    }
    if (root == -1) { return false; }

    // every node has 1 parent, so BFS from root reaches each node at most once
    queue<int> q;
    q.push(root);
    int reached = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        reached++;
        for (int nxt : graph[cur]) { q.push(nxt); }
    }
    return reached == n;
}

// (2) min root making a binary tree
// core idea: tree iff n - 1 edges and BFS reaches all; non-root has degree - 1 children
// so every degree <= 3 and root's <= 2; a tree always has a leaf, so a root exists
// T: O(V + E), S: O(V + E)
int binaryTreeRoot(int n, const vector<vector<int>>& edges) {
    if ((int)edges.size() != n - 1) { return -1; }
    // graph[u] = neighbors of u (undirected edges)
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }

    vector<bool> visited(n, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    int reached = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        reached++;
        for (int nxt : graph[cur]) {
            if (visited[nxt]) { continue; }
            visited[nxt] = true;
            q.push(nxt);
        }
    }
    if (reached != n) { return -1; }

    int root = -1;
    for (int u = 0; u < n; u++) {
        int degree = graph[u].size();
        if (degree > 3) { return -1; }
        if (degree <= 2 && root == -1) { root = u; }
    }
    return root;
}

// (3) roots w/ alternating colors
// core idea: for any root the parent-child pairs = the edges -> 1 color check per edge
// then every node of degree <= 2 is a root
// T: O(V + E), S: O(V + E)
vector<int> alternatingColorRoots(int n, const vector<vector<int>>& edges,
                                  const string& colors) {
    if (binaryTreeRoot(n, edges) == -1) { return {}; }
    // degree[u] = num of edges touching u
    vector<int> degree(n, 0);
    for (const vector<int>& edge : edges) {
        if (colors[edge[0]] == colors[edge[1]]) { return {}; }
        degree[edge[0]]++;
        degree[edge[1]]++;
    }
    vector<int> roots;
    for (int u = 0; u < n; u++) {
        if (degree[u] <= 2) { roots.push_back(u); }
    }
    return roots;
}

// true if BFS from root gives every depth a single color
bool levelsOneColor(const vector<vector<int>>& graph, int root, const string& colors) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(root);
    visited[root] = true;
    while (!q.empty()) {
        int levelSize = q.size();
        // every node of this level must match the first one
        char color = colors[q.front()];
        for (int i = 0; i < levelSize; i++) {
            int cur = q.front();
            q.pop();
            if (colors[cur] != color) { return false; }
            for (int nxt : graph[cur]) {
                if (visited[nxt]) { continue; }
                visited[nxt] = true;
                q.push(nxt);
            }
        }
    }
    return true;
}

// (4) smallest root w/ one color per depth
// core idea: level BFS per candidate root (degree <= 2); a level must match its 1st node
// the color may change between depths
// T: O(V * (V + E)), S: O(V + E)
int uniformDepthColorRoot(int n, const vector<vector<int>>& edges, const string& colors) {
    if (binaryTreeRoot(n, edges) == -1) { return -1; }
    // graph[u] = neighbors of u (undirected edges)
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }
    for (int u = 0; u < n; u++) {
        if (graph[u].size() > 2) { continue; }
        if (levelsOneColor(graph, u, colors)) { return u; }
    }
    return -1;
}

void test(const string& label, const vector<int>& parent, bool want) {
    bool ok = validTreeFromParent(parent) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void test(const string& label, int n, const vector<vector<int>>& edges, int want) {
    bool ok = binaryTreeRoot(n, edges) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void test(const string& label, int n, const vector<vector<int>>& edges,
          const string& colors, const vector<int>& want) {
    bool ok = alternatingColorRoots(n, edges, colors) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testDepthColor(const string& label, int n, const vector<vector<int>>& edges,
                    const string& colors, int want) {
    bool ok = uniformDepthColorRoot(n, edges, colors) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {-1, 0, 1}, true);
    test("two-roots", {-1, -1, 0}, false);
    test("cycle-apart", {-1, 2, 1}, false);
    test("no-root", {1, 2, 0}, false);

    test("path", 3, {{0, 1}, {1, 2}}, 0);
    test("hub-not-root", 4, {{0, 1}, {0, 2}, {0, 3}}, 1);
    test("degree-four", 5, {{0, 1}, {0, 2}, {0, 3}, {0, 4}}, -1);
    test("cycle-plus-loner", 4, {{0, 1}, {1, 2}, {2, 0}}, -1);

    test("striped-path", 3, {{0, 1}, {1, 2}}, "BWB", {0, 1, 2});
    test("striped-star", 4, {{0, 1}, {0, 2}, {0, 3}}, "BWWW", {1, 2, 3});
    test("same-color-edge", 3, {{0, 1}, {1, 2}}, "BBW", {});
    test("striped-cycle", 4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}, "BWBW", {});

    testDepthColor("depth-path-end", 3, {{0, 1}, {1, 2}}, "BWW", 0);
    testDepthColor("depth-skip-center", 3, {{0, 1}, {0, 2}}, "WBW", 1);
    testDepthColor("depth-none", 5, {{0, 1}, {1, 2}, {1, 3}, {3, 4}}, "RRGBR", -1);
    testDepthColor("depth-cycle", 3, {{0, 1}, {1, 2}, {2, 0}}, "BBB", -1);
}

// follow-up:
// (1) w/ union-find -> exactly one -1; union(i, parent[i]) for the rest; same set = cycle
// (1) must also be binary -> reject any node w/ > 2 children after building child lists
// (2) why both checks -> n - 1 edges alone passes cycle + loner
// (2) connected alone -> passes a graph w/ an extra edge
// (4) BFS per root too slow? -> (3) needs none (edge check); (4) tries only degree <= 2
