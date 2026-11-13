// Server graph BFS series (Google, interview report). Undirected, unweighted graph of
// servers; every neighbor is also a key of the map.
// (1) Fewest edges start -> target; 0 if equal, -1 if a server is missing or unreachable.
// (2) Can start reach target using healthy servers only? false if either end is missing
//     or failed; true if start == target and it is healthy.
// (3) Distance from start to every reachable healthy server (start included with 0);
//     empty if start is missing or failed.
// (4) Failure radius: every server within `radius` edges of `failed` (original graph) is
//     disabled; return (3) with the disabled servers. Unknown `failed` disables nothing.
// (5) Number of shortest start -> target paths (long long); 0 if missing or unreachable,
//     1 if start == target.
// (6) Servers on at least one shortest start -> target path, ascending; empty if missing
//     or unreachable.
// (7) Escape: Alice walks from alice to target only along shortest paths; Bob starts at
//     bob at the same time and moves freely, one edge per turn. Alice is caught at v iff
//     Bob can reach v no later than she does (distBob[v] <= distAlice[v], target and her
//     start included). Return whether some shortest path keeps distBob[v] > distAlice[v]
//     at every node; false if target is unreachable. A missing bob never catches her.
//
// Clarify:
// - is every wire listed from both ends? -> yes (undirected); every neighbor is a key
// - start == target -> 0 / true (if healthy)
// - a server not in the map -> -1 / false / empty
// - failed start or target -> false; failed start in (3) -> empty
// - (4): is `failed` itself disabled? -> yes (distance 0 <= radius)
// - (5): many shortest paths? -> count in long long; (7): ties go to Bob (<=), and Bob
//   moves at Alice's speed
// Idea: BFS from start; dist (or seen) is set on push and doubles as the visited set.
//   Failed servers are never pushed. (4) = one BFS from `failed` on the original graph
//   to collect everything within radius, then (3) avoiding those.
//   (5) adds ways[cur] into ways[nxt] on every layer edge; (6) keeps v with distS[v] +
//   distT[v] == distS[target]; (7) BFS over that shortest-path DAG through safe servers.
// Pitfalls:
// - graph.at(cur), not graph[cur]: operator[] does not compile on a const map
// - check missing / failed endpoints before the BFS
// - mark on push (dist[nxt] = dist[cur] + 1 before q.push)
// - (4): the disabled BFS runs on the original graph (nothing skipped)
// - (5): add ways on every edge into the next layer, not only on first discovery
// - (7): Alice's start and target can be caught too; a missing bob catches nothing
// Tests to write: two hops; same server; missing server; isolated server; detour around
//   one failure; both routes cut; failed start; radius 0; radius reaching start.
//   (5)-(6): two equal routes, self, unreachable; (7): Bob behind / ahead / at target.

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

// (5) BFS; every edge cur -> nxt with dist[nxt] == dist[cur] + 1 adds ways[cur] to
// ways[nxt]. BFS pops a whole layer before the next, so ways[cur] is final when popped.
// T: O(V + E), S: O(V).
long long countShortestPaths(const Graph& graph, const string& start,
                             const string& target) {
    if (!graph.count(start) || !graph.count(target)) { return 0; }
    // dist[server] = edges from start
    unordered_map<string, int> dist;
    // ways[server] = number of shortest start -> server paths
    unordered_map<string, long long> ways;
    dist[start] = 0;
    ways[start] = 1;
    queue<string> q;
    q.push(start);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : graph.at(cur)) {
            if (!dist.count(nxt)) {
                dist[nxt] = dist[cur] + 1;
                q.push(nxt);
            }
            if (dist[nxt] == dist[cur] + 1) { ways[nxt] += ways[cur]; }
        }
    }
    return ways.count(target) ? ways[target] : 0;
}

// (6) BFS from both ends; v is on a shortest path iff
// distS[v] + distT[v] == distS[target].
// T: O(V + E + V log V) for the sort, S: O(V).
vector<string> nodesOnShortestPaths(const Graph& graph, const string& start,
                                    const string& target) {
    // edges from start / from target over the full graph
    unordered_map<string, int> distS = healthyDistances(graph, {}, start);
    unordered_map<string, int> distT = healthyDistances(graph, {}, target);
    vector<string> nodes;
    if (!distS.count(target)) { return nodes; }
    for (const auto& [server, fromStart] : distS) {
        // every server reachable from start is reachable from target too
        if (fromStart + distT[server] == distS[target]) { nodes.push_back(server); }
    }
    sort(nodes.begin(), nodes.end());
    return nodes;
}

// Bob catches Alice at server iff he can get there no later than she does
bool caught(const unordered_map<string, int>& distAlice,
            const unordered_map<string, int>& distBob, const string& server) {
    auto it = distBob.find(server);
    return it != distBob.end() && it->second <= distAlice.at(server);
}

// (7) Alice may only step cur -> nxt with distAlice[nxt] == distAlice[cur] + 1 (the
// shortest-path DAG); BFS from alice over that DAG through safe servers only.
// Note: Bob moves at Alice's speed, so catching her at v means he can follow her path
// and also be at target in time; the answer equals distBob[target] > distAlice[target].
// The DAG BFS still works when Bob is slower or barred from some servers.
// T: O(V + E), S: O(V).
bool canEscape(const Graph& graph, const string& alice, const string& bob,
               const string& target) {
    // edges from alice / from bob over the full graph
    unordered_map<string, int> distAlice = healthyDistances(graph, {}, alice);
    unordered_map<string, int> distBob = healthyDistances(graph, {}, bob);
    if (!distAlice.count(target) || caught(distAlice, distBob, alice)) { return false; }
    // safe DAG servers already queued
    unordered_set<string> seen = {alice};
    queue<string> q;
    q.push(alice);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        if (cur == target) { return true; }
        for (const string& nxt : graph.at(cur)) {
            if (distAlice[nxt] != distAlice[cur] + 1 || seen.count(nxt)) { continue; }
            if (caught(distAlice, distBob, nxt)) { continue; }
            seen.insert(nxt);
            q.push(nxt);
        }
    }
    return false;
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

void testCount(const string& label, const Graph& graph, const string& start,
               const string& target, long long want) {
    bool ok = countShortestPaths(graph, start, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testOnPath(const string& label, const Graph& graph, const string& start,
                const string& target, const vector<string>& want) {
    bool ok = nodesOnShortestPaths(graph, start, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testEscape(const string& label, const Graph& graph, const string& alice,
                const string& bob, const string& target, bool want) {
    bool ok = canEscape(graph, alice, bob, target) == want;
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

    testCount("two-routes", graph, "lb", "backup", 2);
    testCount("one-route", graph, "lb", "web1", 1);
    testCount("count-self", graph, "lb", "lb", 1);
    testCount("count-none", graph, "lb", "spare", 0);

    testOnPath("whole-diamond", graph, "lb", "backup",
               {"backup", "db", "lb", "web1", "web2"});
    testOnPath("single-edge", graph, "lb", "web1", {"lb", "web1"});
    testOnPath("on-path-none", graph, "lb", "spare", {});

    testEscape("bob-behind", graph, "web1", "lb", "backup", true);
    testEscape("bob-ahead", graph, "lb", "web1", "backup", false);
    testEscape("bob-at-target", graph, "lb", "backup", "backup", false);
    testEscape("bob-cut-off", graph, "lb", "spare", "backup", true);
    testEscape("same-start", graph, "lb", "lb", "db", false);
}

// follow-up: return the server path, not only its length -> parent[nxt] = cur when nxt
//   is first seen, then walk back from target:
//   for (string v = target; v != start; v = parent[v]) { path.push_back(v); }
//   path.push_back(start);
//   reverse(path.begin(), path.end());
// follow-up: J16 power decays by 1 per wire, many torches -> multi-source BFS: push every
//   torch with dist 0 first; power[node] = max(0, 16 - dist[node])
//   (code: nearestZeroDistance in select/08-graphs/03-rotting-oranges.cpp)
// follow-up: J34 distance to the nearest taxi -> same multi-source BFS on the grid, every
//   taxi cell enqueued with 0, dist set on push
// follow-up: node count on the shortest path avoiding broken nodes -> already (3):
//   dist = healthyDistances(graph, broken, start); count = dist[target] + 1, or -1 if
//   target is not in dist (covers broken start / target and unreachable)
// follow-up: weighted wires -> Dijkstra from start with the same skip (never push a
//   broken server); to report nodes on that path, store hops[server] beside dist
// follow-up: (5) with weighted wires (LC 1976) -> Dijkstra; on a strictly shorter dist
//   reset ways[nxt] = ways[cur], on an equal dist add ways[cur]; counts mod 1e9+7
// follow-up: (7) Bob moves one edge every 2 turns -> caught iff 2 distBob[v] <=
//   distAlice[v]; catching at v no longer implies catching at target, so the DAG BFS
//   is needed (same when Bob may not enter some servers)
