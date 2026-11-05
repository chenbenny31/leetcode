// cycle def?
// rotation of cycle counts? no
// self-loop? count as cycle
// output-format -> {0, 1, 2, 0}
// order of output? any
// size -> num of cycles can be exp

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

//
void dfs(const vector<vector<int>>& graph, int s, int cur,
         vector<bool>& onPath, vector<int>& path, vector<vector<int>>& result) {
    onPath[cur] = true;
    path.push_back(cur);
    for (int nxt : graph[cur]) {
        if (nxt == s) { // record path + s
            result.push_back(path);
            result.back().push_back(s);
        } else if (nxt > s && !onPath[nxt]) {
            dfs(graph, s, nxt, onPath, path, result);
        }
    }
    path.pop_back();
    onPath[cur] = false;
}

// for each start s, DFS, T: O(V! * V), S: O(V)
vector<vector<int>> allCycles(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> onPath(n, false);
    for (int s = 0; s < n; s++) {
        dfs(graph, s, s, onPath, path, result);
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& graph,
          const vector<vector<int>>& want) {
    cout << label << (allCycles(graph) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one cycle", {{1}, {2}, {0, 3}, {}}, {{0, 1, 2, 0}});
    test("shared node", {{1}, {0, 2}, {1}}, {{0, 1, 0}, {1, 2, 1}});
    test("self loop", {{0}}, {{0, 0}});
}
