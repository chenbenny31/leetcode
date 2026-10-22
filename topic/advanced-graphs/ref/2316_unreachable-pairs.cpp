// LC 2316 Count Unreachable Pairs of Nodes: undirected graph on nodes 0..n-1. Return the
// number of pairs of nodes that cannot reach each other.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

int findRoot(vector<int>& uf, int x) {
    while (uf[x] != x) {
        uf[x] = uf[uf[x]];
        x = uf[x];
    }
    return x;
}

// Union-find gives component sizes; pair each component with the nodes in components
// after it, so each pair is counted once. T: O((V+E) * alpha), S: O(V).
long long countPairs(int n, vector<vector<int>>& edges) {
    vector<int> uf(n);   // uf[x] = parent of x in its component
    for (int i = 0; i < n; i++) { uf[i] = i; }
    for (const auto& e : edges) {
        int ra = findRoot(uf, e[0]), rb = findRoot(uf, e[1]);
        uf[ra] = rb;
    }
    vector<int> size(n, 0);   // size[r] = nodes in the component rooted at r
    for (int i = 0; i < n; i++) { size[findRoot(uf, i)]++; }
    long long result = 0;
    long long rest = n;       // nodes in components not yet counted
    for (int r = 0; r < n; r++) {
        rest -= size[r];
        result += rest * size[r];
    }
    return result;
}

void test(const string& label, int n, vector<vector<int>> edges, long long want) {
    cout << label << (countPairs(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-edges", 4, {}, 6);
    test("connected", 3, {{0, 1}, {1, 2}}, 0);
    test("two-groups", 5, {{0, 1}, {2, 3}, {3, 4}}, 6);
    test("big-n", 100000, {}, 4999950000LL);
}
