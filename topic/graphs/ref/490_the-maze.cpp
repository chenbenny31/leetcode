// LC 490 The Maze: a ball rolls in one direction until it hits a wall or the border
// (maze: 0 = empty, 1 = wall). Can it start at `start` and stop exactly at `dest`?
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// BFS over stopping cells; an edge is one full roll in a direction.
// T: O(R*C*(R+C)), S: O(R*C).
bool hasPath(const vector<vector<int>>& maze, const vector<int>& start,
             const vector<int>& dest) {
    int m = maze.size(), n = maze[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<bool>> seen(m, vector<bool>(n, false));   // stopping cells reached
    queue<pair<int, int>> q;
    seen[start[0]][start[1]] = true;
    q.push({start[0], start[1]});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == dest[0] && c == dest[1]) { return true; }
        for (const auto& d : dirs) {
            int nr = r, nc = c;
            // roll while the next cell is inside and empty
            while (nr + d[0] >= 0 && nr + d[0] < m && nc + d[1] >= 0 && nc + d[1] < n &&
                   maze[nr + d[0]][nc + d[1]] == 0) {
                nr += d[0];
                nc += d[1];
            }
            if (seen[nr][nc]) { continue; }
            seen[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return false;
}

void test(const string& label, const vector<vector<int>>& maze,
          const vector<int>& start, const vector<int>& dest, bool want) {
    cout << label << (hasPath(maze, start, dest) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-rolls", {{0, 0, 0}, {1, 1, 0}, {0, 0, 0}}, {0, 0}, {2, 2}, true);
    test("passes-through", {{0, 0, 0}}, {0, 0}, {0, 1}, false);
    test("walled-off", {{0, 1, 0}}, {0, 0}, {0, 2}, false);
    test("wall-stops-ball", {{0, 0, 1}}, {0, 0}, {0, 1}, true);
}
