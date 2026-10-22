// LC 261 Graph Valid Tree: n nodes labeled 0..n-1 and a list of undirected edges.
// Do the edges form a valid tree (connected, no cycle)?
#include <iostream>
#include <vector>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::swap;

struct Dsu {
    vector<int> parent;   // parent[x] = x's parent; a root is its own parent
    vector<int> sz;       // sz[r] = nodes in the tree of root r
    Dsu(int n) : parent(n), sz(n, 1) {
        for (int i = 0; i < n; i++) { parent[i] = i; }
    }
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];   // path halving
            x = parent[x];
        }
        return x;
    }
    // false if a and b were already connected
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) { return false; }
        if (sz[ra] < sz[rb]) { swap(ra, rb); }
        parent[rb] = ra;
        sz[ra] += sz[rb];
        return true;
    }
};

// A tree has exactly n-1 edges; with n-1 edges and no cycle it is connected too.
// Union-find with path halving + union by size. T: O(E * alpha(V)), S: O(V).
bool validTree(int n, const vector<vector<int>>& edges) {
    if ((int)edges.size() != n - 1) { return false; }
    Dsu dsu(n);
    for (const auto& e : edges) {
        if (!dsu.unite(e[0], e[1])) { return false; }
    }
    return true;
}

void test(const string& label, int n, const vector<vector<int>>& edges, bool want) {
    cout << label << (validTree(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("path", 4, {{0, 1}, {1, 2}, {2, 3}}, true);
    test("cycle", 4, {{0, 1}, {1, 2}, {2, 0}}, false);
    test("disconnected", 3, {{0, 1}}, false);
    test("single", 1, {}, true);
}
