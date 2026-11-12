// Server graph BFS series (Google, interview report). Undirected, unweighted graph of
// servers; every neighbor is also a key of the map.
// (1) Fewest edges start -> target; 0 if equal, -1 if a server is missing or unreachable.
// (2) Can start reach target using healthy servers only? false if either end is missing
//     or failed; true if start == target and it is healthy.
// (3) Distance from start to every reachable healthy server (start included with 0);
//     empty if start is missing or failed.
// (4) Failure radius: every server within `radius` edges of `failed` (original graph) is
//     disabled; return (3) with the disabled servers. Unknown `failed` disables nothing.

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

// graph[server] = servers sharing a wire with it
using Graph = unordered_map<string, vector<string>>;

// (1) BFS from start; the first time target is reached is the fewest edges.
// T: O(V + E), S: O(V).
int shortestDistance(const Graph& graph, const string& start, const string& target) {
    if (!graph.count(start) || !graph.count(target)) { return -1; }
    // dist[server] = edges from start, set when first seen
    unordered_map<string, int> dist;
    dist[start] = 0;
    queue<string> q;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (cur == target) { return dist[cur]; }
        for (const string& nxt : graph.at(cur)) {
            if (dist.count(nxt)) { continue; }
            dist[nxt] = dist[cur] + 1;
            q.push(nxt);
        }
    }
    return -1;
}

// (2) BFS from start that never enters a failed server. T: O(V + E), S: O(V).
bool canReach(const Graph& graph, const unordered_set<string>& failed,
              const string& start, const string& target) {
    if (!graph.count(start) || !graph.count(target)) { return false; }
    if (failed.count(start) || failed.count(target)) { return false; }
    // servers already queued
    unordered_set<string> seen = {start};
    queue<string> q;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (cur == target) { return true; }
        for (const string& nxt : graph.at(cur)) {
            if (failed.count(nxt) || seen.count(nxt)) { continue; }
            seen.insert(nxt);
            q.push(nxt);
        }
    }
    return false;
}

// (3) full BFS from start skipping failed servers; dist doubles as the visited set.
// T: O(V + E), S: O(V).
unordered_map<string, int> healthyDistances(const Graph& graph,
                                            const unordered_set<string>& failed,
                                            const string& start) {
    // dist[server] = edges from start over healthy servers
    unordered_map<string, int> dist;
    if (!graph.count(start) || failed.count(start)) { return dist; }
    dist[start] = 0;
    queue<string> q;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : graph.at(cur)) {
            if (failed.count(nxt) || dist.count(nxt)) { continue; }
            dist[nxt] = dist[cur] + 1;
            q.push(nxt);
        }
    }
    return dist;
}

// (4) BFS from `failed` over the original graph gives the disabled servers
// (distance <= radius); then (3) from start avoiding them. T: O(V + E), S: O(V).
unordered_map<string, int> distancesAfterFailure(const Graph& graph, const string& start,
                                                 const string& failed, int radius) {
    // servers within radius of failed
    unordered_set<string> disabled;
    for (const auto& [server, dist] : healthyDistances(graph, {}, failed)) {
        if (dist <= radius) { disabled.insert(server); }
    }
    return healthyDistances(graph, disabled, start);
}

void test(const string& label, const Graph& graph, const string& start,
          const string& target, int want) {
    bool ok = shortestDistance(graph, start, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testReach(const string& label, const Graph& graph,
               const unordered_set<string>& failed, const string& start,
               const string& target, bool want) {
    bool ok = canReach(graph, failed, start, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testAll(const string& label, const Graph& graph, const unordered_set<string>& failed,
             const string& start, const unordered_map<string, int>& want) {
    bool ok = healthyDistances(graph, failed, start) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testRadius(const string& label, const Graph& graph, const string& start,
                const string& failed, int radius,
                const unordered_map<string, int>& want) {
    bool ok = distancesAfterFailure(graph, start, failed, radius) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Graph graph = {{"lb", {"web1", "web2"}},
                   {"web1", {"lb", "db"}},
                   {"web2", {"lb", "db"}},
                   {"db", {"web1", "web2", "backup"}},
                   {"backup", {"db"}},
                   {"spare", {}}};

    test("two-hops", graph, "lb", "db", 2);
    test("same", graph, "lb", "lb", 0);
    test("missing", graph, "lb", "cache", -1);
    test("isolated", graph, "lb", "spare", -1);

    testReach("detour", graph, {"web1"}, "lb", "db", true);
    testReach("cut-off", graph, {"web1", "web2"}, "lb", "db", false);
    testReach("failed-start", graph, {"lb"}, "lb", "lb", false);
    testReach("healthy-self", graph, {}, "lb", "lb", true);

    testAll("skip-failed", graph, {"web2"}, "lb",
            {{"lb", 0}, {"web1", 1}, {"db", 2}, {"backup", 3}});
    testAll("start-down", graph, {"lb"}, "lb", {});

    testRadius("radius-0", graph, "lb", "db", 0, {{"lb", 0}, {"web1", 1}, {"web2", 1}});
    testRadius("start-hit", graph, "lb", "web1", 1, {});
    testRadius("edge-hit", graph, "lb", "backup", 1,
               {{"lb", 0}, {"web1", 1}, {"web2", 1}});
    testRadius("unknown", graph, "lb", "cache", 2,
               {{"lb", 0}, {"web1", 1}, {"web2", 1}, {"db", 2}, {"backup", 3}});
}

// follow-up: J16 power decays by 1 per wire, many torches -> multi-source BFS: push every
//   torch with dist 0 first; power[node] = max(0, 16 - dist[node])
// follow-up: J34 distance to the nearest taxi -> same multi-source BFS on the grid, every
//   taxi cell enqueued with 0, dist set on push
// follow-up: node count on the shortest path avoiding broken nodes -> already (3):
//   dist = healthyDistances(graph, broken, start); count = dist[target] + 1, or -1 if
//   target is not in dist (covers broken start / target and unreachable)
// follow-up: weighted wires -> Dijkstra from start with the same skip (never push a
//   broken server); to report nodes on that path, store hops[server] beside dist
