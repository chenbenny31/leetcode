// could cycle? yes
// input format? adj-list
// start==target? return {{start}}
// un-reachable target -> return {}
// output order? any
// size: upper-bound of n

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// path = nodes from start to cur; onPath[v] = v blocks cycle
void dfs(const vector<vector<int>>& graph, int cur, int target,
         vector<bool>& onPath, vector<int>& path, vector<vector<int>>& result) {
    onPath[cur] = true;
    path.push_back(cur);
    if (cur == target) {
        result.push_back(path);
    } else {
        for (int nxt : graph[cur]) {
            if (!onPath[nxt]) {
                dfs(graph, nxt, target, onPath, path, result);
            }
        }
    }
    path.pop_back();
    onPath[cur] = false;
}
                
// DFS + backtracking, T: O(V! * V), S: O(V)
vector<vector<int>> allPaths(const vector<vector<int>>& graph, int start, int target) {
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> onPath(graph.size(), false);
    dfs(graph, start, target, onPath, path, result);
    return result;
}

void test(const string& label, const vector<vector<int>>& graph, int start, int target,
          const vector<vector<int>>& want) {
    cout << label << (allPaths(graph, start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2}, {3}, {3}, {}}, 0, 3, {{0, 1, 3}, {0, 2, 3}});
    test("cycle", {{1}, {0, 2}, {}}, 0, 2, {{0, 1, 2}});
    test("unreachable", {{1}, {0}, {}}, 0, 2, {});
    test("start is target", {{1}, {0}}, 0, 0, {{0}});
}
