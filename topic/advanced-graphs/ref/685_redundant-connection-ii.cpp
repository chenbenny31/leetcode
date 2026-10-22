// LC 685 Redundant Connection II: a rooted tree on nodes 1..n (edges point parent ->
// child) had one extra directed edge added. Return the edge to remove so a rooted tree
// remains; if several work, return the one that occurs last in the input.
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

// Two cases: a node with two parents (try dropping the later edge; if a cycle still
// exists the earlier edge is the culprit), or only a cycle (the edge closing it).
// T: O(E * alpha), S: O(V).
vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    vector<int> parent(n + 1, 0);   // parent[v] = first parent seen for v, 0 if none
    vector<int> first, second;      // the two edges into one node, in input order
    for (const auto& e : edges) {
        if (parent[e[1]] != 0) {
            first = {parent[e[1]], e[1]};
            second = e;
        } else {
            parent[e[1]] = e[0];
        }
    }
    vector<int> uf(n + 1);          // union-find over edges, ignoring `second`
    for (int i = 0; i <= n; i++) { uf[i] = i; }
    for (const auto& e : edges) {
        if (e == second) { continue; }
        int ru = findRoot(uf, e[0]), rv = findRoot(uf, e[1]);
        if (ru == rv) { return first.empty() ? e : first; }
        uf[ru] = rv;
    }
    return second;
}

void test(const string& label, vector<vector<int>> edges, const vector<int>& want) {
    bool ok = findRedundantDirectedConnection(edges) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cycle-only", {{1, 2}, {2, 3}, {3, 1}}, {3, 1});
    test("two-parents-no-cycle", {{1, 3}, {2, 3}, {1, 2}}, {2, 3});
    test("two-parents-on-cycle", {{2, 1}, {3, 1}, {4, 2}, {1, 4}}, {2, 1});
    test("cycle-off-root", {{1, 2}, {2, 3}, {3, 4}, {4, 2}}, {4, 2});
}
