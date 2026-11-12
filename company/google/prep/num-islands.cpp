// connect cells? up/dn/lf/rt -> followup to diag
// empty grid?
// allow in-place modify of grid?
// grid size -> recur dfs has overflow issue, bfs
// return the island count

// each un-visited cell starts a new land -> bfs from it to mark island cells to '0' before push (no dup enqueue)
// each cell is pushed at most once, whole scan is linear to cell num

// BFS from each un-visited cell, T: O(mn), S: O(mn) for copy
int numIslands(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {0, 0, 1, -1};
    int dc[] = {1, -1, 0, 0};
    int islands = 0;

    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; } // skip marked ones

            islands++;
            grid[r][c] = '0';
            queue<pair<int, int>> q;
            q.push({r, c});

            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0'; // mark before enque
                    q.push({nr, nc});
                }
            }
        }
    }
    return islands;
}

// follow-up: land added on cell at a time, get mxn grid of water and list of positions that each turns a cell into land

// use union-find on cell with id = r * n + c, a new land adds to count, parent[id] == -1 marks water
// add one more land -> count++ -> test if connected, if so count--

int find(vector<int>& parent, int x) { // T: O(log) amortized
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

bool unite(vector<int>& parent, int a, int b) {
    int rootA = find(parent, a);
    int rootB = find(parent, b);
    if (rootA == rootB) { return false; }
    parent[rootB] = rootA;
    return true;
}

// union-find, T: O(mn + klog(mn)), S: O(mn)
vector<int> numIslandsAfterAdds(int m, int n, const vector<vector<int>>& positions) {
    vector<int> parent(m * n, -1); // -1 means water
    int dr[] = {0, 0, 1, -1};
    int dc[] = {1, -1, 0, 0};
    vector<int> result;
    int count = 0;

    for (const auto& pos : positions) {
        int id = pos[0] * n + pos[1];
        if (parent[id] != -1) {
            result.push_back(count);
            continue;
        }

        parent[id] = id;
        count++;
        for (int d = 0; d < 4; d++) {
            int nr = pos[0] + dr[d];
            int nc = pos[1] + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }

            int nxtId = nr * n + nc;
            if (parent[nxtId] == -1) { continue; }
            int rootCur = find(parent, id);
            int rootNxt = find(parent, nxt);
            if (rootCur == rootNxt) { continue; }
            parent[rootNxt] = rootCur;
            count--;
        }
        result.push_back(count);
    }
    return result;
}
