// binary-search + bfs/dfs, T: O((V+E)logV) ~ O(n2logn), V=n^2 cells, E~=4n^2, S: O(V)

#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    int swimInWater(std::vector<std::vector<int>>& grid) {
        int n = static_cast<int>(grid.size());
        int lo = grid[0][0];
        int hi = n * n - 1;

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (can_reach(grid, mid, n)) { hi = mid; }
            else { lo = mid + 1; }
        }
        return lo;
    }

private:
    bool can_reach(std::vector<std::vector<int>>& grid, int t, int n) {
        if (grid[0][0] > t) { return false; }
        std::vector<uint8_t> visited(n * n, 0);
        std::vector<int> stk;
        stk.reserve(n * n);
        stk.push_back(0);
        visited[0] = 1;

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};

        while (!stk.empty()) {
            int idx = stk.back();
            stk.pop_back();
            if (idx == n * n - 1) { return true; }

            int r = idx / n;
            int c = idx % n;
            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                int nidx = nr * n + nc;
                if (visited[nidx] || grid[nr][nc] > t) { continue; }
                visited[nidx] = 1;
                stk.push_back(nidx);
            }
        }
        return false;
    }
};

// modified Dijkstra, T: O(ElogV) ~ O(n2logn), S: O(V)

#include <vector>
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <functional> // std::pair
#include <algorithm> // std::greater
#include <climits> // INT_MAX

class Solution {
public:
    int swimInWater(std::vector<std::vector<int>>& grid) {
        int n = static_cast<int>(grid.size());
        std::vector<int> best(n * n, INT_MAX);
        best[0] = grid[0][0];

        std::vector<std::pair<int, int>> buf;
        buf.reserve(n * n);
        std::priority_queue<std::pair<int, int>,
                           std::vector<std::pair<int, int>>,
                           std::greater<>> pq(std::greater<>{}, std::move(buf));
        pq.push({grid[0][0], 0});

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};

        while (!pq.empty()) {
            auto [t, idx] = pq.top(); pq.pop();
            if (t > best[idx]) { continue; }
            if (idx == n * n - 1) { return t; }

            int r = idx / n;
            int c = idx % n;
            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                int nidx = nr * n + nc;
                int cand = std::max(t, grid[nr][nc]);
                if (cand < best[nidx]) {
                    best[nidx] = cand;
                    pq.push({cand, nidx});
                }
            }
        }
        return -1;
    }
};

// union-find, T: O(ElogE), S: O(V)

#include <vector>
#include <algorithm> // std::sort, std::swap
#include <cstdint> // uint8_t

class Solution {
public:
    int swimInWater(std::vector<std::vector<int>>& grid) {
        int n = static_cast<int>(grid.size());
        int total = n * n;
        std::vector<int> parent(total);
        std::vector<int> rnk(total, 0);
        for (int i = 0; i < total; i++) { parent[i] = i; }

        std::vector<int> order(total);
        for (int i = 0; i < total; i++) { order[i] = i; }
        std::sort(order.begin(), order.end(), [&](int a, int b) {
                return grid[a / n][a % n] < grid[b / n][b % n];
        });

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) { return; }
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            if (rnk[a] == rnk[b]) { rnk[a]++; }
        };

        std::vector<uint8_t> opened(total, 0);
        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};

        for (int idx : order) {
            int r = idx / n;
            int c = idx % n;
            opened[idx] = 1;

            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                int nidx = nr * n + nc;
                if (opened[nidx]) { unite(idx, nidx); }
            }
            if (find(0) == find(total - 1)) { return grid[r][c]; }
        }
        return -1;
    }
};

// modified Dijkstra: max not sum, relaxation is max(dist[u], grid[v]), minimax path
// t > best[idx]: stale entry skip
// flat r*n+c indexing throughout: single contiguous over nested vector for best/visited/opened
// cache behavior: Dijkstra heap touches scattered entries; uf op are O(1) amortized and flat-array friendly
