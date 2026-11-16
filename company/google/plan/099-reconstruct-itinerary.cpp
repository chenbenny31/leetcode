// LC 332 Reconstruct Itinerary (Hard) — Eulerian path (Hierholzer)
// tickets[i] = {from, to}; return route from "JFK" using every ticket exactly once
// several valid routes -> smallest in lexical order
//
// clarify:
// valid route always exists -> no existence check
// dup tickets -> ok, each is its own edge
// tie rule -> lexical order
// start -> always "JFK"

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

// post-order walk; appends cur once all its tickets are used
void dfs(unordered_map<string, vector<string>>& graph, const string& cur,
         vector<string>& route) {
    vector<string>& dests = graph[cur];
    while (!dests.empty()) {
        string nxt = dests.back();
        dests.pop_back();
        dfs(graph, nxt, route);
    }
    // post-order: add cur only when it has no unused ticket left
    route.push_back(cur);
}

// core idea: Hierholzer: fly smallest unused ticket; append airport once it is stuck
// dead end is appended first -> lands last after the reverse
// T: O(E log E), S: O(E)
vector<string> findItinerary(const vector<vector<string>>& tickets) {
    // graph[from] = unused dests, sorted descending (smallest at the back)
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

// follow-up:
// why post-order -> greedy walk can enter a dead end too early; post-order puts it last
// Eulerian path exists? -> all in == out, or one out - in == 1 and one in - out == 1
// also needed for existence -> all edges in one connected component
// valid arrangement of pairs (LC 2097) -> same walk on ints; start at out - in == 1
// cracking the safe (LC 753) -> nodes = k^(n-1) prefixes, edges = codes; Eulerian circuit
// iterative -> stack: top has unused ticket ? push dest : pop top into route; reverse
