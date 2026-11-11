// LC 2050 Parallel Courses III (Hard). Pattern: topological order + longest path (DAG).
// Courses 1..n, relations [prev, next] = prev must finish before next starts, course i
// takes time[i-1] months; any number run at once. Return the min months to finish all.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::max;

// Kahn's order; a course starts when its last prereq finishes. T: O(V+E), S: O(V+E).
int minimumTime(int n, const vector<vector<int>>& relations, const vector<int>& time) {
    // graph[u] = courses that wait for u (0-based)
    vector<vector<int>> graph(n);
    // indeg[v] = prereqs of v not finished yet
    vector<int> indeg(n, 0);
    for (const vector<int>& rel : relations) {
        int before = rel[0] - 1;
        int after = rel[1] - 1;
        graph[before].push_back(after);
        indeg[after]++;
    }
    // start[v] = latest finish among v's prereqs
    vector<int> start(n, 0);
    queue<int> q;
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
    }
    int total = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        int finish = start[cur] + time[cur];
        total = max(total, finish);
        for (int nxt : graph[cur]) {
            start[nxt] = max(start[nxt], finish);
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return total;
}

void test(const string& label, int n, const vector<vector<int>>& relations,
          const vector<int>& time, int want) {
    cout << label << (minimumTime(n, relations, time) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 2}, {1, 3}, {2, 4}, {3, 4}}, {1, 5, 2, 1}, 7);
    test("chain", 3, {{1, 2}, {2, 3}}, {1, 2, 3}, 6);
    test("independent", 2, {}, {5, 1}, 5);
}

// follow-up: every course takes 1 unit (LC 1136) -> Kahn level by level, count levels;
//   return -1 if a cycle leaves courses untaken
// follow-up: critical courses -> parent[v] = prereq that set start[v]; walk back
