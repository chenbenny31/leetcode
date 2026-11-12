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
//
// Clarify (1):
// - parent[i] is 0..n-1, or -1 for a root; empty array -> no root -> false
// - more than 2 children allowed? -> yes, (1) only asks "is it one tree"
// - self-loop parent[i] == i? -> stated none; it would stay unreached and fail anyway
// Idea (1): exactly one -1, then BFS down the child lists from it. Each node has one
//   parent, so no node is pushed twice, and a node on a cycle (or under one) is never
//   reached: valid iff reached == n.
// Pitfalls (1): zero roots (everything on cycles) and two roots are separate checks; no
//   visited array is needed only because every node has exactly one parent.
// Clarify (2):
// - undirected edges on 0..n-1; duplicate edges / self-loops -> with n - 1 edges they
//   leave the graph disconnected, so the BFS catches them
// - several valid roots -> the smallest; none -> -1; n = 1, no edges -> 0
// - binary = at most 2 children; a non-root of degree d has d - 1 children
// Idea (2): a tree iff n - 1 edges and connected (one BFS). Rooted anywhere, a non-root
//   needs degree <= 3 and the root degree <= 2. A tree always has a leaf, so if every
//   degree is <= 3 the smallest node of degree <= 2 works.
// Pitfalls (2):
// - both checks: n - 1 edges alone passes a cycle plus a loner; connected alone passes
//   a graph with an extra edge
// - undirected BFS needs visited, marked on push
// - any degree > 3 means -1, even when some node of degree <= 2 exists
// Clarify (3): colors 'B' / 'W'; return every valid root ascending; none -> {}.
// Idea (3): whatever the root, the parent-child pairs are exactly the edges, so the color
//   rule is one check per edge; after that every node of degree <= 2 is a root.
// Pitfalls (3): no BFS per candidate root (O(V^2)); the color check ignores the root.
// Tests to write: (1) chain; two roots; cycle away from the root; no root. (2) path; star
//   whose hub cannot be the root; degree 4; n - 1 edges as a cycle plus a loner.
//   (3) striped path; striped star; one same-color edge; striped 4-cycle.

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

// follow-up (J30): same graph, colors[i] any char. Return the smallest root that makes it
// a binary tree as in (2) with every depth a single color, -1 if none. Level BFS from
// each candidate (degree <= 2), checking each level against its first node.
// T: O(V * (V + E)), S: O(V + E).
// Pitfall: the color may change from one depth to the next; only a level must agree.
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

// follow-up (J30): every depth one color -> uniformDepthColorRoot above
// follow-up: (1) with union-find -> one -1, and union(i, parent[i]) for every other i; a
//   union inside one set is a cycle. n - 1 links with no cycle => connected
//   (uf[i] = i at the start, findRoot as in select/08-graphs/01):
//     int a = findRoot(uf, i);
//     int b = findRoot(uf, parent[i]);
//     if (a == b) { return false; }
//     uf[a] = b;
// follow-up: (1) must also be binary -> after building the child lists:
//     if (graph[p].size() > 2) { return false; }
