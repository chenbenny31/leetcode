// Package install order: [package, dependency] means package needs dependency first.
// Return an install order for target and all its transitive dependencies (dependencies
// visited in input order), or {} if a cycle is reachable from target.
// Follow-up: Kahn's algorithm on the closure.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

// post-order DFS: install cur after all its dependencies. Meeting a package that is
// still on the current path (state 1) means a cycle.
bool dfs(const string& cur, const unordered_map<string, vector<string>>& graph,
         unordered_map<string, int>& state, vector<string>& order) {
    state[cur] = 1;
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
    state[cur] = 2;
    order.push_back(cur);
    return true;
}

// DFS from target only, so unrelated packages (and their cycles) are ignored.
// T: O(V+E), S: O(V+E).
vector<string> packageInstallOrder(const vector<vector<string>>& dependencies,
                                   const string& target) {
    // graph[p] = packages p depends on, in input order
    unordered_map<string, vector<string>> graph;
    for (const auto& d : dependencies) { graph[d[0]].push_back(d[1]); }
    // state[p]: 0 = not visited, 1 = on the current DFS path, 2 = installed
    unordered_map<string, int> state;
    vector<string> order;
    bool ok = dfs(target, graph, state, order);
    if (!ok) { return {}; }
    return order;
}

// follow-up: Kahn on only what target needs. First collect target's dependency closure,
// then repeatedly install a package whose dependencies are all installed. Packages left
// over sit on (or behind) a cycle. T: O(V+E), S: O(V+E).
vector<string> installOrderKahn(const vector<vector<string>>& dependencies,
                                const string& target) {
    // graph[p] = packages p depends on
    unordered_map<string, vector<string>> graph;
    for (const auto& d : dependencies) { graph[d[0]].push_back(d[1]); }
    // need = target and everything it transitively depends on
    unordered_set<string> need{target};
    queue<string> q;
    q.push(target);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        auto it = graph.find(cur);
        if (it == graph.end()) { continue; }
        for (const string& nxt : it->second) {
            if (need.insert(nxt).second) { q.push(nxt); }
        }
    }
    // users[d] = needed packages that depend on d (edge d -> user means d first)
    unordered_map<string, vector<string>> users;
    // waiting[p] = dependencies of p not installed yet
    unordered_map<string, int> waiting;
    for (const string& p : need) {
        waiting[p] = 0;
        auto it = graph.find(p);
        if (it == graph.end()) { continue; }
        for (const string& d : it->second) {
            users[d].push_back(p);
            waiting[p]++;
        }
    }
    queue<string> ready;
    for (const string& p : need) {
        if (waiting[p] == 0) { ready.push(p); }
    }
    vector<string> order;
    while (!ready.empty()) {
        string cur = ready.front();
        ready.pop();
        order.push_back(cur);
        for (const string& nxt : users[cur]) {
            waiting[nxt]--;
            if (waiting[nxt] == 0) { ready.push(nxt); }
        }
    }
    // some needed package never became ready: a cycle inside the closure
    if (order.size() < need.size()) { return {}; }
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

// follow-up: very deep chains -> same post-order with an explicit stack of
//   (package, next dependency index) frames; only if the interviewer asks
