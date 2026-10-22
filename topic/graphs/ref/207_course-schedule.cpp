// LC 207 Course Schedule: n courses, prerequisite [a, b] = take b before a.
// Can all courses be finished?
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS on indegrees; courses never freed sit on a cycle. T: O(V+E), S: O(V+E).
bool canFinish(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);   // graph[b] = courses unlocked by b
    vector<int> indeg(n, 0);        // indeg[a] = prereqs of a not yet taken
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }
    queue<int> q;                   // courses with every prereq taken
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
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

void test(const string& label, int n, const vector<vector<int>>& prereqs, bool want) {
    cout << label << (canFinish(n, prereqs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("cycle", 3, {{1, 0}, {2, 1}, {0, 2}}, false);
    test("no-prereqs", 1, {}, true);
}
