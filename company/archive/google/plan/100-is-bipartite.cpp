// LC 785 Is Graph Bipartite? (Medium). Pattern: BFS 2-coloring.
// graph[u] = neighbors of node u (undirected, maybe disconnected). Can the nodes be
// split into two sets so every edge joins nodes from different sets?

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS from every uncolored node; neighbors get the other color. T: O(V+E), S: O(V).
bool isBipartite(const vector<vector<int>>& graph) {
    int n = graph.size();
    // color[u] = 0 / 1, -1 = not visited yet
    vector<int> color(n, -1);
    for (int start = 0; start < n; start++) {
        if (color[start] != -1) { continue; }
        color[start] = 0;
        // colored nodes whose neighbors are unchecked
        queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (color[nxt] == -1) {
                    color[nxt] = 1 - color[cur];
                    q.push(nxt);
                } else if (color[nxt] == color[cur]) {
                    return false;
                }
            }
        }
    }
    return true;
}

void test(const string& label, const vector<vector<int>>& graph, bool want) {
    cout << label << (isBipartite(graph) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", {{1, 3}, {0, 2}, {1, 3}, {0, 2}}, true);
    test("triangle", {{1, 2}, {0, 2}, {0, 1}}, false);
    test("odd-far", {{1}, {0}, {3, 4}, {2, 4}, {2, 3}}, false);
    test("isolated", {{}}, true);
}

// follow-up: edge-list input / dislike pairs (LC 886) -> build graph both ways, same BFS
// follow-up: union-find -> for each u, union all its neighbors; u in the same set as a
//   neighbor = not bipartite, O(E * alpha)
