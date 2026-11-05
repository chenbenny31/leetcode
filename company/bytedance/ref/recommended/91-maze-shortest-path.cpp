// ByteDance (jobright, intern phone): maze of 0 (open) / 1 (wall); move up, down, left,
// right. Return the fewest moves from start to end, or -1 if unreachable.

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

// BFS from start; the first time end is reached is the shortest. T: O(R*C), S: O(R*C).
int shortestPath(const vector<vector<int>>& maze, const vector<int>& start,
                 const vector<int>& end) {
    int m = maze.size();
    int n = maze[0].size();
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    // dist[r][c] = moves from start, -1 = not reached yet
    vector<vector<int>> dist(m, vector<int>(n, -1));
    queue<pair<int, int>> q;
    q.push({start[0], start[1]});
    dist[start[0]][start[1]] = 0;
    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        if (r == end[0] && c == end[1]) {
            return dist[r][c];
        }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                continue;
            }
            if (maze[nr][nc] == 1 || dist[nr][nc] != -1) {
                continue;
            }
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& maze, const vector<int>& start,
          const vector<int>& end, int want) {
    cout << label << (shortestPath(maze, start, end) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("around-wall", {{0, 1, 0}, {0, 1, 0}, {0, 0, 0}}, {0, 0}, {0, 2}, 6);
    test("blocked", {{0, 1}, {1, 0}}, {0, 0}, {1, 1}, -1);
    test("same-cell", {{0}}, {0, 0}, {0, 0}, 0);
}

// follow-up: return the path -> parent[r][c] = cell it came from, walk back from end
// follow-up: break up to k walls (LC 1293) -> BFS state (r, c, wallsLeft), O(R*C*k)
// follow-up: ball rolls until a wall (LC 505) -> neighbors are stop cells, edge weight =
//   cells rolled; Dijkstra on dist[r][c]
// follow-up: weighted cells -> Dijkstra; 0/1 weights -> 0-1 BFS with deque
