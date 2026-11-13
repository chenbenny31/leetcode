// LC 332 Reconstruct Itinerary (Hard) — Eulerian path (Hierholzer)
// tickets[i] = {from, to}. Return the route that starts at "JFK", uses every ticket
// exactly once, and is the smallest in lexical order among the valid routes.
// Clarify: a valid route always exists; duplicate tickets; tie rule (lexical order);
//   start is always "JFK".

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::reverse;
using std::greater;

void dfs(unordered_map<string, vector<string>>& graph, const string& cur,
         vector<string>& route) {
    vector<string>& dests = graph[cur];
    while (!dests.empty()) {
        string nxt = dests.back();
        dests.pop_back();
        dfs(graph, nxt, route);
    }
    // post-order: cur is added only when it has no unused ticket left
    route.push_back(cur);
}

// Core idea: Hierholzer: always fly the smallest unused ticket, and append an airport
// only after all its tickets are used, so a dead end lands last; reverse at the end.
// T: O(E log E), S: O(E).
vector<string> findItinerary(const vector<vector<string>>& tickets) {
    // graph[from] = unused destinations, sorted descending (smallest at the back)
    unordered_map<string, vector<string>> graph;
    for (const vector<string>& ticket : tickets) {
        graph[ticket[0]].push_back(ticket[1]);
    }
    for (auto& [from, dests] : graph) {
        sort(dests.begin(), dests.end(), greater<string>());
    }
    // airports in reverse order of the final route
    vector<string> route;
    dfs(graph, "JFK", route);
    reverse(route.begin(), route.end());
    return route;
}

void test(const string& label, const vector<vector<string>>& tickets,
          const vector<string>& want) {
    cout << label << (findItinerary(tickets) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dead-end-last", {{"JFK", "BOS"}, {"JFK", "SEA"}, {"SEA", "JFK"}},
         {"JFK", "SEA", "JFK", "BOS"});
    test("smallest-first",
         {{"JFK", "SEA"}, {"JFK", "BOS"}, {"BOS", "JFK"}, {"SEA", "JFK"}},
         {"JFK", "BOS", "JFK", "SEA", "JFK"});
    test("duplicate-tickets", {{"JFK", "BOS"}, {"BOS", "JFK"}, {"JFK", "BOS"}},
         {"JFK", "BOS", "JFK", "BOS"});
    test("single-ticket", {{"JFK", "SEA"}}, {"JFK", "SEA"});
    return 0;
}

// follow-up (ideas only):
// - why post-order -> the greedy walk can enter a dead end too early; a node is appended
//   only when stuck, so the dead end is appended first and ends up last after the reverse
// - does an Eulerian path exist -> every node has in == out, or exactly one node has
//   out - in == 1 (start) and one has in - out == 1 (end); edges in one component
// - valid arrangement of pairs (LC 2097) -> same walk on ints; start at the node with
//   out - in == 1 if there is one, else anywhere; output consecutive pairs
// - cracking the safe / de Bruijn sequence (LC 753) -> nodes = the k^(n-1) prefixes,
//   edges = the n-digit codes; an Eulerian circuit spells every code once
// - iterative version -> explicit stack: while the top has an unused ticket push its
//   destination, else pop the top into the route; reverse at the end
