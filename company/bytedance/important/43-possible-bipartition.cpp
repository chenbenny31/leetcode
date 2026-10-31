// ByteDance (FastPrep, phone) / LC 886: n people are numbered 1..n; every pair [a, b]
// in dislikes must be in different groups. Can everyone be split into two groups?
// The graph may be disconnected, so every component must be checked. (The round was on
// HackerRank: compile and write your own tests.)

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// BFS 2-coloring from every uncolored person: neighbors get the other color, and a
// neighbor that already has the same color means an odd cycle. T: O(V + E), S: O(V + E).
bool possibleBipartition(int n, const vector<vector<int>>& dislikes) {
    // graph[u] = people u dislikes (undirected)
    vector<vector<int>> graph(n + 1);
    for (const auto& d : dislikes) {
        graph[d[0]].push_back(d[1]);
        graph[d[1]].push_back(d[0]);
    }

    // color[u] = 0 / 1 group, -1 = not visited
    vector<int> color(n + 1, -1);
    for (int start = 1; start <= n; start++) {
        if (color[start] != -1) {
            continue;
        }
        color[start] = 0;
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

void test(const string& label, int n, const vector<vector<int>>& dislikes, bool want) {
    cout << label << (possibleBipartition(n, dislikes) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even-cycle", 4, {{1, 2}, {2, 3}, {3, 4}, {4, 1}}, true);
    test("odd-cycle", 5, {{1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 1}}, false);
    test("bad-second-part", 5, {{1, 2}, {3, 4}, {4, 5}, {5, 3}}, false);
    test("no-edges", 3, {}, true);
}
