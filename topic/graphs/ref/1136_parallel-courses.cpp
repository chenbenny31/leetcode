// LC 1136 Parallel Courses: courses 1..n, relations [prev, next] = prev must be taken
// before next. Any number of unlocked courses can be taken per semester. Return the
// fewest semesters to take all courses, or -1 if there is a cycle.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS layer by layer; one layer = one semester. T: O(V+E), S: O(V+E).
int minimumSemesters(int n, const vector<vector<int>>& relations) {
    vector<vector<int>> graph(n + 1);   // graph[prev] = courses unlocked by prev
    vector<int> indeg(n + 1, 0);        // indeg[c] = prereqs of c not yet taken
    for (const auto& r : relations) {
        graph[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    queue<int> q;                       // courses takeable this semester or the next
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int semesters = 0, taken = 0;
    while (!q.empty()) {
        semesters++;
        // freeze the layer size: courses pushed now belong to the next semester
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            taken++;
            for (int nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    // untaken courses sit on a cycle or behind one
    return taken == n ? semesters : -1;
}

void test(const string& label, int n, const vector<vector<int>>& relations, int want) {
    cout << label << (minimumSemesters(n, relations) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-prereqs", 3, {{1, 3}, {2, 3}}, 2);
    test("shortcut-edge", 3, {{1, 2}, {2, 3}, {1, 3}}, 3);
    test("cycle", 3, {{1, 2}, {2, 3}, {3, 2}}, -1);
    test("no-relations", 2, {}, 1);
}
