// Affected targets for a change (ASE CI: decide what to rebuild and retest). deps[i] =
// {target, dependency}: target depends on dependency. Given the targets changed in a
// commit, return every target that must be rebuilt (the changed ones plus everything
// that depends on them, directly or transitively), sorted by name.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;
using std::sort;

// BFS on the reversed dependency graph from the changed targets.
// T: O(V + E), S: O(V + E).
vector<string> affectedTargets(const vector<vector<string>>& deps,
                               const vector<string>& changed) {
    // graph[d] = targets that depend on d (edges point downstream)
    unordered_map<string, vector<string>> graph;
    for (const vector<string>& d : deps) {
        graph[d[1]].push_back(d[0]);
    }
    unordered_set<string> seen(changed.begin(), changed.end());
    queue<string> q;
    for (const string& t : changed) {
        q.push(t);
    }
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : graph[cur]) {
            // insert(...).second: true only the first time nxt is reached
            if (seen.insert(nxt).second) {
                q.push(nxt);
            }
        }
    }
    vector<string> result(seen.begin(), seen.end());
    sort(result.begin(), result.end());
    return result;
}

void test(const string& label, const vector<string>& changed,
          const vector<string>& want) {
    // net and ui depend on base; app depends on net and ui; cli depends on ui
    vector<vector<string>> deps = {
        {"net", "base"}, {"ui", "base"}, {"app", "net"}, {"app", "ui"}, {"cli", "ui"}};
    cout << label << (affectedTargets(deps, changed) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("leaf-change", {"net"}, {"app", "net"});
    test("root-change", {"base"}, {"app", "base", "cli", "net", "ui"});
    test("top-change", {"cli"}, {"cli"});
    test("two-changes", {"net", "cli"}, {"app", "cli", "net"});
}

// follow-up: build order for the affected set -> Kahn's topological sort restricted to
//   the affected targets (see 03)
// follow-up: tests to run -> tests are targets too; filter the affected set by a test tag
// follow-up: changed files, not targets -> map file -> owning target first (a hash map
//   from path prefix), then the same BFS
