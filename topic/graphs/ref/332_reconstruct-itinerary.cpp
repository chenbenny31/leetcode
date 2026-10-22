// LC 332 Reconstruct Itinerary: use every ticket [from, to] exactly once, starting at
// "JFK". Return the lexicographically smallest valid itinerary; one always exists.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::multiset;
using std::reverse;

// Hierholzer: an airport is appended only after all its tickets are used, so a dead
// end found early lands at the END of the route instead of cutting it short.
void dfs(map<string, multiset<string>>& graph, const string& cur, vector<string>& route) {
    while (!graph[cur].empty()) {
        // take and burn the smallest remaining ticket out of cur
        string nxt = *graph[cur].begin();
        graph[cur].erase(graph[cur].begin());
        dfs(graph, nxt, route);
    }
    route.push_back(cur);
}

// Eulerian path via post-order DFS, smallest ticket first. T: O(E log E), S: O(V+E).
vector<string> findItinerary(const vector<vector<string>>& tickets) {
    map<string, multiset<string>> graph;   // graph[a] = unused destinations from a
    for (const auto& t : tickets) { graph[t[0]].insert(t[1]); }
    vector<string> route;                  // airports in reverse visiting order
    dfs(graph, "JFK", route);
    reverse(route.begin(), route.end());
    return route;
}

void test(const string& label, const vector<vector<string>>& tickets,
          const vector<string>& want) {
    cout << label << (findItinerary(tickets) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("smallest-first", {{"JFK", "B"}, {"JFK", "A"}, {"A", "JFK"}},
         {"JFK", "A", "JFK", "B"});
    test("dead-end-first", {{"JFK", "A"}, {"JFK", "B"}, {"B", "JFK"}},
         {"JFK", "B", "JFK", "A"});
    test("one-ticket", {{"JFK", "ZZZ"}}, {"JFK", "ZZZ"});
}
