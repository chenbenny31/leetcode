// LC 323 Number of Connected Components in an Undirected Graph: n nodes labeled 0..n-1
// and a list of undirected edges. Return the number of connected components.
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

// Start with n components; every successful union merges two.
// Union-find with path halving + union by size. T: O(E * alpha(V)), S: O(V).
int countComponents(int n, const vector<vector<int>>& edges) {
    Dsu dsu(n);
    int comps = n;
    for (const auto& e : edges) {
        if (dsu.unite(e[0], e[1])) { comps--; }
    }
    return comps;
}

void test(const string& label, int n, const vector<vector<int>>& edges, int want) {
    cout << label << (countComponents(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-groups", 5, {{0, 1}, {1, 2}, {3, 4}}, 2);
    test("no-edges", 3, {}, 3);
    test("cycle-merges-once", 3, {{0, 1}, {1, 2}, {2, 0}}, 1);
}
