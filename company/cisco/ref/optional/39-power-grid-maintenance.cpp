// 39 · Power Grid Maintenance (Cisco LC tag, Medium) — LC 3607, union-find + ordered sets
// stations 1..c, all online; connections join stations into grids
// query [1, x]: x if x is online, else the smallest online id in x's grid, -1 if none
// query [2, x]: x goes offline (stays in its grid)
// return the answers of the type-1 queries in order
//
// clarify:
// ids -> 1-based
// a station taken offline twice -> second time is a no-op
// offline stations still connect their grid -> yes, connectivity never changes
// stations come back online -> no (see follow-up)

#include <iostream>
#include <vector>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::vector;
using std::set;

// T: ~O(1) amortized
int findRoot(vector<int>& parent, int x) {
    while (parent[x] != x) {
        // path halving
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

// core idea: grids never change, so union once; per grid an ordered set of online ids
// T: O((c + q) log c), S: O(c)
vector<int> processQueries(int c, const vector<vector<int>>& connections,
                           const vector<vector<int>>& queries) {
    // parent[x] = union-find parent of station x (idx 0 unused)
    vector<int> parent(c + 1);
    for (int i = 0; i <= c; i++) { parent[i] = i; }
    for (const auto& edge : connections) {
        int ra = findRoot(parent, edge[0]);
        int rb = findRoot(parent, edge[1]);
        if (ra != rb) { parent[ra] = rb; }
    }
    // online[root] = online station ids of that grid, smallest first
    vector<set<int>> online(c + 1);
    for (int i = 1; i <= c; i++) { online[findRoot(parent, i)].insert(i); }

    vector<int> out;
    for (const auto& query : queries) {
        int type = query[0];
        int x = query[1];
        set<int>& grid = online[findRoot(parent, x)];
        if (type == 2) {
            grid.erase(x);
            continue;
        }
        if (grid.count(x) > 0) {
            out.push_back(x);
        } else if (grid.empty()) {
            out.push_back(-1);
        } else {
            out.push_back(*grid.begin());
        }
    }
    return out;
}

void test(const string& label, int c, const vector<vector<int>>& connections,
          const vector<vector<int>>& queries, const vector<int>& want) {
    cout << label
         << (processQueries(c, connections, queries) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", 5, {{1, 2}, {2, 3}, {3, 4}, {4, 5}},
         {{1, 3}, {2, 1}, {1, 1}, {2, 2}, {1, 2}}, {3, 2, 3});
    test("isolated", 3, {}, {{1, 1}, {2, 1}, {1, 1}}, {1, -1});
    test("two-grids", 4, {{1, 2}, {3, 4}}, {{2, 3}, {1, 3}, {1, 1}}, {4, 1});
    return 0;
}

// follow-up:
// 1. stations come back online -> insert the id back into its grid's set, nothing else
// 2. no ordered set -> min-heap per grid + offline flags, pop offline tops lazily
// 3. connections added between queries -> union + merge the smaller set into the larger
// 4. connections removed -> process queries backwards so removals become unions
// 5. only "is any station online in x's grid" -> a counter per root is enough
