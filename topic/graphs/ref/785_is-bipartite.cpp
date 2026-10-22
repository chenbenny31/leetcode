// LC 785 Is Graph Bipartite?: graph[u] lists the neighbors of u in an undirected graph
// (possibly disconnected). Can the nodes be split into two sets so that every edge
// joins the two sets?
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS 2-coloring from every uncolored node; an edge inside one color is an odd cycle.
// T: O(V+E), S: O(V).
bool isBipartite(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> color(n, -1);   // color[u] = 0 or 1, -1 = not visited
    for (int s = 0; s < n; s++) {
        if (color[s] != -1) { continue; }
        queue<int> q;           // nodes colored but not expanded yet
        color[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (color[nxt] == color[cur]) { return false; }
                if (color[nxt] != -1) { continue; }
                color[nxt] = 1 - color[cur];
                q.push(nxt);
            }
        }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& graph, bool want) {
    cout << label << (isBipartite(graph) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even-cycle", {{1, 3}, {0, 2}, {1, 3}, {0, 2}}, true);
    test("triangle", {{1, 2}, {0, 2}, {0, 1}}, false);
    test("odd-cycle-in-second-part", {{1}, {0}, {3, 4}, {2, 4}, {2, 3}}, false);
    test("no-edges", {{}, {}}, true);
}
