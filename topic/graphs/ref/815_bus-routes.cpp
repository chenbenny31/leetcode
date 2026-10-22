// LC 815 Bus Routes: routes[i] is the list of stops bus i loops through forever. Starting
// at stop `source` (on no bus yet), return the fewest buses to ride to reach `target`,
// or -1 if impossible.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;

// BFS over buses (not stops): an edge joins two buses that share a stop.
// T: O(total stops), S: O(total stops).
int numBusesToDestination(const vector<vector<int>>& routes, int source, int target) {
    if (source == target) { return 0; }
    int n = routes.size();
    unordered_map<int, vector<int>> stopBuses;   // stop -> buses that visit it
    for (int b = 0; b < n; b++) {
        for (int stop : routes[b]) { stopBuses[stop].push_back(b); }
    }
    vector<int> rides(n, 0);   // rides[b] = buses ridden to board b; 0 = not boarded
    queue<int> q;              // boarded buses whose stops are not scanned yet
    for (int b : stopBuses[source]) {
        rides[b] = 1;
        q.push(b);
    }
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int stop : routes[cur]) {
            if (stop == target) { return rides[cur]; }
            for (int nxt : stopBuses[stop]) {
                if (rides[nxt] != 0) { continue; }
                rides[nxt] = rides[cur] + 1;
                q.push(nxt);
            }
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& routes, int source, int target,
          int want) {
    cout << label << (numBusesToDestination(routes, source, target) == want ? " PASS\n"
                                                                            : " FAIL\n");
}

int main() {
    test("same-stop", {{1, 2}}, 1, 1, 0);
    test("direct-shortcut", {{1, 2}, {2, 3}, {1, 3}}, 1, 3, 1);
    test("transfer", {{1, 2}, {2, 3}}, 1, 3, 2);
    test("unreachable", {{1, 2}, {3, 4}}, 1, 4, -1);
}
