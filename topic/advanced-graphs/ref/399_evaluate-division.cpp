// LC 399 Evaluate Division: equations a / b = v. For each query x / y return its value,
// or -1.0 if it cannot be determined (unknown variable or no chain linking x and y).
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::fabs;

// Weighted union-find find with path compression; afterwards ratio[x] = x / root.
// T: O(alpha) amortized, S: O(1) extra.
int findRoot(vector<int>& parent, vector<double>& ratio, int x) {
    if (parent[x] == x) { return x; }
    int p = parent[x];
    int root = findRoot(parent, ratio, p);
    // ratio[p] is now p / root, so x / root = (x / p) * (p / root)
    ratio[x] *= ratio[p];
    parent[x] = root;
    return root;
}

// Union every equation, then answer a query from the two ratios to the shared root.
// T: O((E + Q) * alpha), S: O(V).
vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values,
                            vector<vector<string>>& queries) {
    unordered_map<string, int> id;   // variable name -> index
    for (const auto& eq : equations) {
        for (const auto& name : eq) {
            if (id.count(name) == 0) {
                int k = id.size();
                id[name] = k;
            }
        }
    }
    int v = id.size();
    vector<int> parent(v);      // parent[x] = next node toward x's root
    vector<double> ratio(v, 1.0);   // ratio[x] = x / parent[x]
    for (int i = 0; i < v; i++) { parent[i] = i; }
    for (int i = 0; i < (int)equations.size(); i++) {
        int a = id[equations[i][0]], b = id[equations[i][1]];
        int ra = findRoot(parent, ratio, a), rb = findRoot(parent, ratio, b);
        if (ra == rb) { continue; }
        parent[ra] = rb;
        // ra / rb = (a / b) * (b / rb) / (a / ra)
        ratio[ra] = values[i] * ratio[b] / ratio[a];
    }
    vector<double> result;
    for (const auto& q : queries) {
        if (id.count(q[0]) == 0 || id.count(q[1]) == 0) {
            result.push_back(-1.0);
            continue;
        }
        int x = id[q[0]], y = id[q[1]];
        if (findRoot(parent, ratio, x) != findRoot(parent, ratio, y)) {
            result.push_back(-1.0);
            continue;
        }
        result.push_back(ratio[x] / ratio[y]);
    }
    return result;
}

void test(const string& label, vector<vector<string>> equations, vector<double> values,
          vector<vector<string>> queries, const vector<double>& want) {
    vector<double> got = calcEquation(equations, values, queries);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        ok = fabs(got[i] - want[i]) < 1e-9;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"x", "y"}, {"y", "z"}}, {2.0, 4.0}, {{"x", "z"}, {"z", "x"}},
         {8.0, 0.125});
    test("two-groups", {{"x", "y"}, {"p", "q"}}, {2.0, 5.0}, {{"x", "q"}, {"q", "p"}},
         {-1.0, 0.2});
    test("unknown-var", {{"x", "y"}}, {3.0}, {{"x", "x"}, {"w", "w"}, {"x", "w"}},
         {1.0, -1.0, -1.0});
    test("join-late", {{"a", "b"}, {"c", "d"}, {"b", "d"}}, {2.0, 3.0, 5.0},
         {{"a", "c"}}, {10.0 / 3.0});
}
