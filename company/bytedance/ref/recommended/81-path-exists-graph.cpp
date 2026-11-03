// ByteDance / LC 1971: an undirected graph has n vertices 0..n-1 and edges[i] = [u, v]
// (may have cycles, duplicate edges, several components). Return whether a path joins
// source and destination. A vertex is always connected to itself.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Root of x, halving the path on the way up.
int findRoot(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

// Union-find: merge both ends of every edge, then compare the two roots.
// T: O((n + E) * alpha(n)), S: O(n).
bool validPath(int n, const vector<vector<int>>& edges, int source, int destination) {
    // parent[v] = v's parent in the union-find forest
    vector<int> parent(n);
    for (int v = 0; v < n; v++) {
        parent[v] = v;
    }
    for (const vector<int>& e : edges) {
        int a = findRoot(parent, e[0]);
        int b = findRoot(parent, e[1]);
        parent[a] = b;
    }
    return findRoot(parent, source) == findRoot(parent, destination);
}

void test(const string& label, int n, const vector<vector<int>>& edges, int source,
          int destination, bool want) {
    bool ok = validPath(n, edges, source, destination) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-component", 4, {{0, 1}, {2, 3}}, 0, 1, true);
    test("other-component", 4, {{0, 1}, {2, 3}}, 0, 3, false);
    test("self", 2, {}, 1, 1, true);
    test("duplicate-edges", 3, {{0, 1}, {1, 0}, {1, 2}}, 0, 2, true);
}
