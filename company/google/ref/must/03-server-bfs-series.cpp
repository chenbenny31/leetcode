// Server graph BFS series (Google, interview report)
// undirected, unweighted graph of servers; every neighbor is also a key of the map
// (1) fewest edges start -> target; -1 if a server is missing or unreachable
// (2) can start reach target using healthy servers only?
// (3) dist from start to every reachable healthy server
// (4) servers within `radius` edges of `failed` (itself included) are disabled; then (3)
// (5) num of shortest start -> target paths
// (6) servers on any shortest path, sorted
// (7) Alice walks only shortest paths to target, Bob moves freely at the same speed
// (7) can she avoid every server Bob reaches no later than she does?
//
// clarify:
// start == target -> dist 0, reachable, 1 path
// unknown server -> -1 / false / empty
// failed start or target -> unreachable
// ties in (7) -> go to Bob (caught)

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

// (1) shortest distance
// core idea: BFS from start; dist set on push = visited set; first pop of target wins
// T: O(V + E), S: O(V)
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

// (2) reachable over healthy servers
// core idea: same BFS, never push a failed server
// T: O(V + E), S: O(V)
bool canReach(const Graph& graph, const unordered_set<string>& failed,
              const string& start, const string& target) {
    if (!graph.count(start) || !graph.count(target)) { return false; }
    if (failed.count(start) || failed.count(target)) { return false; }
    // seen = servers already queued
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

// (3) dist to every healthy server
// core idea: full BFS from start skipping failed servers; dist = visited set
// T: O(V + E), S: O(V)
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

// (4) dist after a failure w/ blast radius
// core idea: BFS from `failed` on the full graph -> disabled = dist <= radius; then (3)
// T: O(V + E), S: O(V)
unordered_map<string, int> distancesAfterFailure(const Graph& graph, const string& start,
                                                 const string& failed, int radius) {
    // disabled = servers within radius of failed
    unordered_set<string> disabled;
    for (const auto& [server, dist] : healthyDistances(graph, {}, failed)) {
        if (dist <= radius) { disabled.insert(server); }
    }
    return healthyDistances(graph, disabled, start);
}

// (5) count shortest paths
// core idea: BFS; every edge cur -> nxt into the next layer adds ways[cur] to ways[nxt]
// layer is fully popped before the next -> ways[cur] is final when popped
// T: O(V + E), S: O(V)
long long countShortestPaths(const Graph& graph, const string& start,
                             const string& target) {
    if (!graph.count(start) || !graph.count(target)) { return 0; }
    // dist[server] = edges from start
    unordered_map<string, int> dist;
    // ways[server] = num of shortest start -> server paths
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

// (6) servers on any shortest path
// core idea: BFS from both ends; v on a shortest path iff distS[v] + distT[v] == distS[t]
// T: O(V log V + E), S: O(V) — log from sorting the output
vector<string> nodesOnShortestPaths(const Graph& graph, const string& start,
                                    const string& target) {
    // distS / distT[server] = edges from start / from target on the full graph
    unordered_map<string, int> distS = healthyDistances(graph, {}, start);
    unordered_map<string, int> distT = healthyDistances(graph, {}, target);
    vector<string> nodes;
    if (!distS.count(target)) { return nodes; }
    for (const auto& [server, fromStart] : distS) {
        // reachable from start -> reachable from target too (undirected)
        if (fromStart + distT[server] == distS[target]) { nodes.push_back(server); }
    }
    sort(nodes.begin(), nodes.end());
    return nodes;
}

// Bob catches Alice at server iff he gets there no later than she does
bool caught(const unordered_map<string, int>& distAlice,
            const unordered_map<string, int>& distBob, const string& server) {
    auto it = distBob.find(server);
    return it != distBob.end() && it->second <= distAlice.at(server);
}

// (7) can Alice escape Bob
// core idea: Alice steps only cur -> nxt w/ distAlice[nxt] == distAlice[cur] + 1 (DAG)
// BFS from alice over that DAG through servers where Bob does not catch her
// T: O(V + E), S: O(V)
bool canEscape(const Graph& graph, const string& alice, const string& bob,
               const string& target) {
    // distAlice / distBob[server] = edges from alice / from bob on the full graph
    unordered_map<string, int> distAlice = healthyDistances(graph, {}, alice);
    unordered_map<string, int> distBob = healthyDistances(graph, {}, bob);
    if (!distAlice.count(target) || caught(distAlice, distBob, alice)) { return false; }
    // seen = safe DAG servers already queued
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
    testEscape("bob-cut-off", graph, "lb", "spare", "backup", true);
    testEscape("same-start", graph, "lb", "lb", "db", false);
}

// follow-up:
// return the path, not only its len -> parent[nxt] = cur on first seen, walk back
// many torches, power decays 1 per wire (J16) -> multi-source BFS; power = 16 - dist
// dist to nearest taxi (J34) -> same multi-source BFS on the grid
// node cnt on shortest path avoiding broken nodes -> (3) w/ broken as failed; dist + 1
// weighted wires -> Dijkstra from start w/ the same skip (never push a broken server)
// (5) weighted (LC 1976) -> Dijkstra; shorter dist resets ways, equal adds; mod 1e9+7
// (7) at the same speed -> answer is simply distBob[target] > distAlice[target]
// (7) Bob moves 1 edge per 2 turns -> caught iff 2 * distBob[v] <= distAlice[v]; DAG BFS
