// LC 684 Redundant Connection: a tree on nodes 1..n plus one extra edge. Return the
// extra edge; if several qualify, the one that occurs last in the input.
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

// The first edge whose ends are already connected closes the cycle; it is the last
// such edge in input order. Union-find with path halving + union by size.
// T: O(E * alpha(V)), S: O(V).
vector<int> findRedundantConnection(const vector<vector<int>>& edges) {
    int n = edges.size();   // a tree on n nodes has n-1 edges, plus one extra
    Dsu dsu(n + 1);
    for (const auto& e : edges) {
        if (!dsu.unite(e[0], e[1])) { return e; }
    }
    return {};
}

void test(const string& label, const vector<vector<int>>& edges,
          const vector<int>& want) {
    cout << label << (findRedundantConnection(edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("triangle", {{1, 2}, {2, 3}, {1, 3}}, {1, 3});
    test("late-cycle", {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 5}}, {4, 1});
    test("two-nodes", {{1, 2}, {1, 2}}, {1, 2});
}
