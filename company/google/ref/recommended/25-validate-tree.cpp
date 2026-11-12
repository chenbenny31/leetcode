// Validate Tree (Google, interview report), two parts.
// (1) parent[i] = parent of node i, -1 marks the root (no self-loops). Valid iff there is
//     exactly one root and every node is reachable from it (no cycles, no stray parts).
// (2) n nodes 0..n-1, undirected edges. Return a root that makes the graph a binary tree
//     (every node has <= 2 children), or -1 if none. Needs a tree (n - 1 edges and
//     connected), every degree <= 3 (1 parent + 2 children), root degree <= 2.
//     Returns the smallest valid root.
// (3) Same graph plus colors[i] = 'B' / 'W'. Return every root (ascending) that makes it
//     a binary tree as in (2) AND gives every parent-child pair different colors; empty
//     if none.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// (1) build child lists, BFS from the single root, count reached nodes. A node on a cycle
// never connects to the root, so it stays unreached. T: O(n), S: O(n).
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

    // every node has one parent, so BFS from the root reaches each node at most once
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

// (2) tree = n - 1 edges + BFS reaches all; then any node of degree <= 2 can be the root
// as long as no degree exceeds 3. T: O(V + E), S: O(V + E).
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

// (3) parent-child pairs are exactly the tree edges whatever the root, so the color rule
// is a property of each edge, not of the root. (2) returns -1 iff the graph is not a tree
// or some degree > 3; otherwise every node of degree <= 2 is a valid root.
// T: O(V + E), S: O(V + E).
vector<int> alternatingColorRoots(int n, const vector<vector<int>>& edges,
                                  const string& colors) {
    if (binaryTreeRoot(n, edges) == -1) { return {}; }
    // degree[u] = number of edges touching u
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
}

// follow-up (J30): each node colored, every depth one color -> for each candidate root
//   (degree <= 2) BFS level by level and check all nodes of a level share the first one's
//   color; return true on the first root that passes. O(V * (V + E)) worst.
// follow-up: (1) with union-find -> union child with parent; a union inside one set is a
//   cycle; valid iff one root and no cycle (n - 1 links + acyclic => connected).
