// Package installer: deps[p] lists the packages p needs. install(p, payload) installs
// every missing dependency first, then p, and passes payload to p's install step.
// Already-installed packages are skipped; a dependency cycle makes the install fail.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

// DFS post-order is a topological order (dependencies first); a package seen again
// while still on the DFS path means a cycle. T: O(V + E), S: O(V).
// (Concurrency: packages whose dependencies are all installed can install in parallel,
// level by level, as in Kahn's algorithm.)
class Installer {
    // deps[p] = packages p depends on
    unordered_map<string, vector<string>> deps;
    unordered_set<string> installed;
    // order[i] = i-th package actually installed
    vector<string> order;
    // payloadOf[p] = payload handed to p's install step
    unordered_map<string, string> payloadOf;

    // false on a cycle; onPath = packages on the current DFS path
    bool dfs(const string& p, unordered_set<string>& onPath) {
        if (installed.count(p)) { return true; }
        if (onPath.count(p)) { return false; }
        onPath.insert(p);
        if (deps.count(p)) {
            for (const string& d : deps[p]) {
                if (!dfs(d, onPath)) { return false; }
            }
        }
        onPath.erase(p);
        installed.insert(p);
        order.push_back(p);
        return true;
    }

public:
    void addPackage(const string& p, const vector<string>& needs) { deps[p] = needs; }

    bool install(const string& p, const string& payload) {
        unordered_set<string> onPath;
        if (!dfs(p, onPath)) { return false; }
        payloadOf[p] = payload;
        return true;
    }

    const vector<string>& installOrder() const { return order; }
    string payload(const string& p) const {
        if (!payloadOf.count(p)) { return ""; }
        return payloadOf.at(p);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Installer inst;
    inst.addPackage("app", {"web", "log"});
    inst.addPackage("web", {"ssl"});
    check(inst.install("app", "prod"), "installs");
    vector<string> want = {"ssl", "web", "log", "app"};
    check(inst.installOrder() == want, "deps-first");
    check(inst.payload("app") == "prod", "payload");
    check(inst.install("web", "x") && inst.installOrder().size() == 4, "skip-installed");
    Installer loop;
    loop.addPackage("a", {"b"});
    loop.addPackage("b", {"a"});
    check(!loop.install("a", ""), "cycle");
}
