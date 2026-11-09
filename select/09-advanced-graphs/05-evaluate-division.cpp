// LC 399 Evaluate Division (Medium). Pattern: weighted graph DFS, multiply along path.
// equations a / b = v. For each query x / y return its value, or -1.0 if it cannot be
// determined (unknown variable or no chain linking x and y).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::unordered_set;
using std::fabs;

// returns cur / target along some path, or -1.0 if target is unreachable
double dfs(unordered_map<string, vector<pair<string, double>>>& graph,
           const string& cur, const string& target, unordered_set<string>& seen) {
    if (cur == target) { return 1.0; }
    seen.insert(cur);
    for (const auto& [nxt, ratio] : graph[cur]) {
        if (seen.count(nxt)) { continue; }
        double rest = dfs(graph, nxt, target, seen);
        if (rest >= 0) { return ratio * rest; }
    }
    return -1.0;
}

// Edge num -> den weighs num / den (den -> num weighs the inverse); DFS per query.
// T: O(Q * (V+E)), S: O(V+E).
vector<double> calcEquation(const vector<vector<string>>& equations,
                            const vector<double>& values,
                            const vector<vector<string>>& queries) {
    // graph[num] = (den, num / den) for every known ratio, both directions
    unordered_map<string, vector<pair<string, double>>> graph;
    for (int i = 0; i < (int)equations.size(); i++) {
        const string& num = equations[i][0];
        const string& den = equations[i][1];
        graph[num].push_back({den, values[i]});
        graph[den].push_back({num, 1.0 / values[i]});
    }
    vector<double> result;
    for (const vector<string>& query : queries) {
        const string& num = query[0];
        const string& den = query[1];
        // x / x = 1 only for a known x; an unknown variable has no value at all
        if (!graph.count(num) || !graph.count(den)) {
            result.push_back(-1.0);
            continue;
        }
        // variables visited in this query's DFS
        unordered_set<string> seen;
        result.push_back(dfs(graph, num, den, seen));
    }
    return result;
}

void test(const string& label, const vector<vector<string>>& equations,
          const vector<double>& values, const vector<vector<string>>& queries,
          const vector<double>& want) {
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

// follow-up: many queries -> weighted union-find, weight[x] = x / root; x / y =
//   weight[x] / weight[y] when same root, ~O(alpha) per query
// follow-up: dense, fixed variables -> Floyd-Warshall on ratios, O(V^3) then O(1)
