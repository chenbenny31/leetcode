// LC 1197 Minimum Knight Moves: a knight starts at (0, 0) on an infinite chessboard.
// Return the fewest moves to reach (x, y).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::abs;

// BFS on a bounded window: by symmetry use |x|, |y|; a shortest path never leaves
// [-2, x+2] x [-2, y+2]. T: O(x * y), S: O(x * y).
int minKnightMoves(int x, int y) {
    x = abs(x);
    y = abs(y);
    const int moves[8][2] = {{1, 2}, {2, 1}, {2, -1}, {1, -2},
                             {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};
    // dist[i + 2][j + 2] = moves to reach (i, j); -1 = not reached
    vector<vector<int>> dist(x + 5, vector<int>(y + 5, -1));
    queue<pair<int, int>> q;
    dist[2][2] = 0;
    q.push({0, 0});
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        if (cx == x && cy == y) { return dist[cx + 2][cy + 2]; }
        for (const auto& mv : moves) {
            int nx = cx + mv[0], ny = cy + mv[1];
            if (nx < -2 || nx > x + 2 || ny < -2 || ny > y + 2) { continue; }
            if (dist[nx + 2][ny + 2] != -1) { continue; }
            dist[nx + 2][ny + 2] = dist[cx + 2][cy + 2] + 1;
            q.push({nx, ny});
        }
    }
    return -1;
}

void test(const string& label, int x, int y, int want) {
    cout << label << (minKnightMoves(x, y) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("origin", 0, 0, 0);
    test("one-move-mirrored", -2, -1, 1);
    test("diagonal-neighbor", 1, 1, 2);
    test("far-diagonal", 5, 5, 4);
}
