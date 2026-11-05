// ByteDance (hack2hire) / LC 547: n photos; similar[i][j] = 1 if photos i and j are
// similar. Similar photos, directly or through a chain, form a group. Count the groups.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS from each unvisited photo; row cur is its adjacency list. T: O(n^2), S: O(n).
int countGroups(const vector<vector<int>>& similar) {
    int n = similar.size();
    // seen[i] = photo i already placed in a group
    vector<bool> seen(n, false);
    int groups = 0;
    for (int i = 0; i < n; i++) {
        if (seen[i]) {
            continue;
        }
        groups++;
        // photos of this group still to expand
        queue<int> q;
        q.push(i);
        seen[i] = true;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt = 0; nxt < n; nxt++) {
                if (similar[cur][nxt] == 1 && !seen[nxt]) {
                    seen[nxt] = true;
                    q.push(nxt);
                }
            }
        }
    }
    return groups;
}

struct DSU {
    // parent[x] = x's parent; a root is its own parent
    vector<int> parent;
    // number of disjoint sets
    int groups;

    DSU(int n) : parent(n), groups(n) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // path compression: point x straight at its root
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) {
            return;
        }
        parent[ra] = rb;
        groups--;
    }
};

// follow-up: union every similar pair in the upper triangle.
// T: O(n^2 log n) (path compression only), S: O(n).
int countGroupsDSU(const vector<vector<int>>& similar) {
    int n = similar.size();
    DSU dsu(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (similar[i][j] == 1) {
                dsu.unite(i, j);
            }
        }
    }
    return dsu.groups;
}

void test(const string& label, const vector<vector<int>>& similar, int want) {
    bool ok = countGroups(similar) == want && countGroupsDSU(similar) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {{1, 1, 0}, {1, 1, 0}, {0, 0, 1}}, 2);
    test("all-alone", {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, 3);
    test("chain", {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, 1);
}

// follow-up: union-find -> see countGroupsDSU() above; groups-- on each real merge
// follow-up: edge list input (LC 323) -> graph[u] adjacency list + BFS, O(V + E);
//   or DSU over edges, O(E alpha(n)) with union by size
// follow-up: pairs arrive online, groups after each -> DSU unite per pair, read groups
// follow-up: largest group -> DSU size[root], or count photos per BFS; take max
