// numCourses 0..n-1, pre-req [a, b] means take b before a
// return if all courses can be finished

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS, a cycle leaves nodes never reaching indeg 0, T: O(V+E), S: O(V+E)
bool canFinish(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n); // i -> courses dep on i
    vector<int> indeg(n, 0); // indeg[i] = remain deps of i
    for (const auto& p : prereqs) {
        int from = p[1];
        int to = p[0];
        graph[from].push_back(to);
        indeg[to]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    int taken = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        taken++;

        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return taken == n;
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          bool want) {
    cout << label << (canFinish(n, prerequisites) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("cycle", 2, {{1, 0}, {0, 1}}, false);
    test("part-cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, false);
    test("no-pre", 1, {}, true);
}
