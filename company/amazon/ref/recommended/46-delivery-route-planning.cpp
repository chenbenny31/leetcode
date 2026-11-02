// A robot on a grid (0 = free, 1 = obstacle, 4-directional unit moves) starts at start
// and must finish jobs; job j = (pickup cell, drop-off cell), and each package must be
// picked up before it is dropped off. It may carry at most capacity packages at once.
// Return the min total distance, or -1 if impossible.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::min;

const int INF = INT_MAX / 2;

// BFS distances from src to every cell (INF if unreachable)
vector<vector<int>> bfs(const vector<vector<int>>& grid, pair<int, int> src) {
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> dist(m, vector<int>(n, INF));
    int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1};
    queue<pair<int, int>> q;
    auto [sr, sc] = src;
    dist[sr][sc] = 0;
    q.push(src);
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n || grid[nr][nc] == 1) { continue; }
            if (dist[nr][nc] != INF) { continue; }
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return dist;
}

// 1) BFS from each key point (start, pickups, drop-offs) gives pairwise distances.
// 2) DP over (job states, robot position): each job is 0 = waiting, 1 = carried,
//    2 = delivered, packed in base 3. Every move raises one job's digit, so states can
//    be processed in increasing code order. T: O(K * mn + 3^J * K * J) for K = 2J + 1
//    key points, S: O(3^J * K + K^2).
int minRoute(const vector<vector<int>>& grid, pair<int, int> start,
             const vector<pair<pair<int, int>, pair<int, int>>>& jobs, int capacity) {
    int J = jobs.size();
    // points[0] = start, points[1 + 2j] = pickup j, points[2 + 2j] = drop-off j
    vector<pair<int, int>> points = {start};
    for (const auto& [pickup, drop] : jobs) {
        points.push_back(pickup);
        points.push_back(drop);
    }
    int K = points.size();
    // dist[a][b] = grid distance between key points a and b
    vector<vector<int>> dist(K, vector<int>(K));
    for (int a = 0; a < K; a++) {
        vector<vector<int>> d = bfs(grid, points[a]);
        for (int b = 0; b < K; b++) {
            auto [r, c] = points[b];
            dist[a][b] = d[r][c];
        }
    }
    vector<int> pow3(J + 1, 1);
    for (int j = 1; j <= J; j++) { pow3[j] = pow3[j - 1] * 3; }
    // best[code][p] = min distance to reach job states code with the robot at point p
    vector<vector<int>> best(pow3[J], vector<int>(K, INF));
    best[0][0] = 0;
    for (int code = 0; code < pow3[J]; code++) {
        int carried = 0;
        for (int j = 0; j < J; j++) {
            if (code / pow3[j] % 3 == 1) { carried++; }
        }
        for (int p = 0; p < K; p++) {
            if (best[code][p] == INF) { continue; }
            for (int j = 0; j < J; j++) {
                int state = code / pow3[j] % 3;
                if (state == 2 || (state == 0 && carried == capacity)) { continue; }
                // next stop: the pickup if waiting, the drop-off if carried
                int q = state == 0 ? 1 + 2 * j : 2 + 2 * j;
                if (dist[p][q] == INF) { continue; }
                int next = code + pow3[j];
                best[next][q] = min(best[next][q], best[code][p] + dist[p][q]);
            }
        }
    }
    int answer = INF;
    for (int p = 0; p < K; p++) { answer = min(answer, best[pow3[J] - 1][p]); }
    if (answer == INF) { return -1; }
    return answer;
}

void test(const string& label, const vector<vector<int>>& grid, pair<int, int> start,
          const vector<pair<pair<int, int>, pair<int, int>>>& jobs, int capacity,
          int want) {
    bool ok = minRoute(grid, start, jobs, capacity) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> line = {{0, 0, 0, 0, 0}};
    test("one-job", line, {0, 0}, {{{0, 3}, {0, 1}}}, 1, 5);
    test("order", line, {0, 1}, {{{0, 0}, {0, 1}}, {{0, 4}, {0, 3}}}, 2, 6);
    test("carry-two", line, {0, 0}, {{{0, 1}, {0, 4}}, {{0, 2}, {0, 3}}}, 2, 4);
    test("carry-one", line, {0, 0}, {{{0, 1}, {0, 4}}, {{0, 2}, {0, 3}}}, 1, 7);
    test("detour", {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}, {0, 1}, {{{2, 1}, {1, 0}}}, 1, 6);
    test("blocked", {{0, 1, 0}}, {0, 0}, {{{0, 2}, {0, 0}}}, 1, -1);
}
