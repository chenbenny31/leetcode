// LC 542 01 Matrix: for every cell of a binary matrix, return the distance to the
// nearest 0 (4-directional steps).
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

// Multi-source BFS from every 0 at once; first visit is the shortest distance.
// T: O(R*C), S: O(R*C).
vector<vector<int>> updateMatrix(const vector<vector<int>>& mat) {
    int m = mat.size(), n = mat[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<int>> dist(m, vector<int>(n, -1));   // -1 = not reached yet
    queue<pair<int, int>> q;                           // reached cells to expand
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (mat[r][c] != 0) { continue; }
            dist[r][c] = 0;
            q.push({r, c});
        }
    }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (const auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (dist[nr][nc] != -1) { continue; }
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return dist;
}

void test(const string& label, const vector<vector<int>>& mat,
          const vector<vector<int>>& want) {
    cout << label << (updateMatrix(mat) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("row", {{0, 1, 1, 1}}, {{0, 1, 2, 3}});
    test("pit", {{0, 0, 0}, {0, 1, 0}, {1, 1, 1}}, {{0, 0, 0}, {0, 1, 0}, {1, 2, 1}});
    test("all-zero", {{0}}, {{0}});
}
