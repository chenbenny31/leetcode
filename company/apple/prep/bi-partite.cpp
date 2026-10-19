// undirected graph nodes 0..n-1, edge list [u, v]
// return if the nodes can be 2-colored

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// bfs from every un-colored node, color neighbor before enqueue
// T: O(V+E), S: O(V+E)
bool isBipartite(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n); // node -> list of neibors
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }

    vector<int> color(n, -1); // -1 means not visited, 0/1 are diff colors
    for (int i = 0; i < n; i++) {
        if (color[i] != -1) { continue; }
        color[i] = 0;
        queue<int> q;
        q.push(i);
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

void test(const string& label, int n, const vector<vector<int>>& edges, bool want) {
    cout << label << (isBipartite(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", 4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}, true);
    test("triangle", 3, {{0, 1}, {1, 2}, {2, 0}}, false);
    test("odd cycle far", 5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}}, false);
    test("no edges", 1, {}, true);
}
