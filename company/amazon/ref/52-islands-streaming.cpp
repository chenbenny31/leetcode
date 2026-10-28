// Count islands (4-connected groups of 1s) in a 0/1 grid.
// Streaming version: rows arrive one at a time and the whole grid is never stored.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

void dfs(vector<vector<int>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 1) { return; }
    grid[r][c] = 0;
    dfs(grid, r + 1, c);
    dfs(grid, r - 1, c);
    dfs(grid, r, c + 1);
    dfs(grid, r, c - 1);
}

// straightforward: whole grid in memory, sink each island with DFS. T: O(mn), S: O(mn).
int numIslands(vector<vector<int>> grid) {
    int count = 0;
    int m = grid.size(), n = grid[0].size();
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 1) {
                count++;
                dfs(grid, r, c);
            }
        }
    }
    return count;
}

// streaming: label each land cell, union it with its left and upper neighbors; only the
// previous row's labels are kept. islands = labels created - successful unions.
// T: O(cells * alpha), S: O(n + land cells) for the union-find.
class IslandCounter {
    // parent[x] = union-find parent of label x
    vector<int> parent;
    // prevRow[c] = label of the cell above column c, -1 for water
    vector<int> prevRow;
    int count = 0;

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) { return; }
        parent[ra] = rb;
        count--;
    }

public:
    void addRow(const vector<int>& row) {
        int n = row.size();
        if (prevRow.empty()) { prevRow.assign(n, -1); }
        vector<int> cur(n, -1);
        for (int c = 0; c < n; c++) {
            if (row[c] != 1) { continue; }
            cur[c] = parent.size();
            parent.push_back(cur[c]);
            count++;
            if (c > 0 && cur[c - 1] != -1) { unite(cur[c], cur[c - 1]); }
            if (prevRow[c] != -1) { unite(cur[c], prevRow[c]); }
        }
        prevRow = cur;
    }

    int islands() const { return count; }
};

void test(const string& label, const vector<vector<int>>& grid, int want) {
    IslandCounter stream;
    for (const auto& row : grid) { stream.addRow(row); }
    bool ok = numIslands(grid) == want && stream.islands() == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {{1, 0, 1}}, 2);
    test("u-shape", {{1, 0, 1}, {1, 1, 1}}, 1);
    test("diag", {{1, 0}, {0, 1}}, 2);
    test("water", {{0, 0}}, 0);
    test("merge-late", {{1, 0, 1, 0, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1}}, 1);
}
