// graph[s] = services that depend on s (a DAG). Given services shut down directly, a
// service is affected if it is shut down or any service it depends on is affected.
// Return all affected services.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::set;
using std::unordered_map;

// multi-source BFS along "is depended on by" edges. T: O(V + E), S: O(V).
set<string> affected(const unordered_map<string, vector<string>>& graph,
                     const vector<string>& shutdown) {
    set<string> seen(shutdown.begin(), shutdown.end());
    queue<string> q;
    for (const string& s : shutdown) { q.push(s); }
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (!graph.count(cur)) { continue; }
        for (const string& nxt : graph.at(cur)) {
            if (seen.insert(nxt).second) { q.push(nxt); }
        }
    }
    return seen;
}

void test(const string& label, const unordered_map<string, vector<string>>& graph,
          const vector<string>& shutdown, const set<string>& want) {
    cout << label << (affected(graph, shutdown) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    unordered_map<string, vector<string>> diamond = {
        {"db", {"api", "jobs"}}, {"api", {"web"}}, {"jobs", {"web"}}, {"web", {}},
        {"cdn", {"web"}}};
    test("root", diamond, {"db"}, {"db", "api", "jobs", "web"});
    test("middle", diamond, {"api"}, {"api", "web"});
    test("leaf", diamond, {"web"}, {"web"});
    test("two", diamond, {"jobs", "cdn"}, {"jobs", "cdn", "web"});
}
