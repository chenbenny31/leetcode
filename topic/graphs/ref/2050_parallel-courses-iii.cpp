// LC 2050 Parallel Courses III: courses 1..n form a DAG via relations [prev, next];
// course i takes time[i - 1] months. Unlimited courses run in parallel once their
// prereqs are done. Return the fewest months to finish all courses.
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

// Kahn's BFS; a course starts when its slowest prereq finishes (longest path in a DAG).
// T: O(V+E), S: O(V+E).
int minimumTime(int n, const vector<vector<int>>& relations, const vector<int>& time) {
    vector<vector<int>> graph(n + 1);   // graph[prev] = courses unlocked by prev
    vector<int> indeg(n + 1, 0);        // indeg[c] = prereqs of c not yet finished
    for (const auto& r : relations) {
        graph[r[0]].push_back(r[1]);
        indeg[r[1]]++;
    }
    vector<int> start(n + 1, 0);        // start[c] = latest finish among c's prereqs
    queue<int> q;                       // courses whose prereqs are all finished
    for (int c = 1; c <= n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    int total = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        // start[cur] is final: every prereq was popped before cur
        int finish = start[cur] + time[cur - 1];
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
    test("parallel-prereqs", 3, {{1, 3}, {2, 3}}, {3, 2, 5}, 8);
    test("chain", 3, {{1, 2}, {2, 3}}, {1, 2, 3}, 6);
    test("no-relations", 2, {}, {4, 7}, 7);
}
