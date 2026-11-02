// edge direction -> [pkg, dep], dep -> pkg
// several valid orders? any one
// has cycle -> return [] if reachable from target
// dup pairs? assume none
// size? 1e5 pkgs -> explicit stack

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// post-order DFS: install cur after its deps
bool dfs(const string& cur, const unordered_map<string, vector<string>>& graph,
         unordered_map<string, int>& state, vector<string>& order) {
    state[cur] = 1; // on-path
    auto it = graph.find(cur);
    if (it != graph.end()) {
        for (const string& nxt : it->second) {
            if (state[nxt] == 1) { return false; }
            if (state[nxt] == 0) {
                bool ok = dfs(nxt, graph, state, order);
                if (!ok) { return false; }
            }
        }
    }
    state[cur] = 2; // visited
    order.push_back(cur);
    return true;
}

// DFS from target over parent graph, T: O(V+E), S: O(V+E)
vector<string> packageInstallOrder(const vector<vector<string>>& dependencies,
                                   const string& target) {
    unordered_map<string, vector<string>> parent; // parent[cur] -> [cur's-deps, ]
    for (const auto& d : dependencies) {
        string dep = d[1];
        string pkg = d[0];
        parent[pkg].push_back(dep);
    }
    unordered_map<string, int> state; // 0 for not visited, 1 on path, 2 visited
    vector<string> order;
    bool ok = dfs(target, parent, state, order);
    if (!ok) { return {}; }
    return order;
}

void test(const string& label, const vector<vector<string>>& dependencies,
          const string& target, const vector<string>& want) {
    bool ok = packageInstallOrder(dependencies, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{"app", "log"}, {"app", "net"}, {"log", "core"}, {"net", "core"}},
         "app", {"core", "log", "net", "app"});
    test("cycle", {{"a", "b"}, {"b", "a"}}, "a", {});
    test("cycle-elsewhere", {{"a", "b"}, {"x", "y"}, {"y", "x"}}, "a", {"b", "a"});
    test("no-deps", {}, "solo", {"solo"});
}
