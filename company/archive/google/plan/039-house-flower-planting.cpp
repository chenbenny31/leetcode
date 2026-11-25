// Houses and Flowers (Google, interview report)
// grid of 'H' (house) and '.' (soil); plant flowers 'F' on soil
// rule: at most 1 flower per row and per col
// rule: every house has exactly 1 flower among its 4 neighbors
// rule: no flower touches 2 houses
// (1) one valid planting, {} if impossible
// (2) every valid planting
// reported: x2 (early-career NG onsite: decide, then all plantings; L4 onsite: N x N)
//
// clarify:
// flower shared by 2 houses -> not allowed (each house owns its flower)
// flowers that belong to no house -> none planted here; the L4 version needs them
// neighbors -> 4 directions
// no house -> the unchanged grid is the answer
// several answers in (1) -> any

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// num of houses next to (r, c)
int housesAround(const vector<string>& grid, int r, int c) {
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;
    for (const auto& dir : dirs) {
        int nr = r + dir[0];
        int nc = c + dir[1];
        if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == 'H') { count++; }
    }
    return count;
}

// gives houses[idx..] a flower each; appends finished grids to out until it holds limit
void dfs(vector<string>& grid, const vector<pair<int, int>>& houses, size_t idx,
         vector<bool>& rowUsed, vector<bool>& colUsed, size_t limit,
         vector<vector<string>>& out) {
    if (out.size() == limit) { return; }
    if (idx == houses.size()) {
        out.push_back(grid);
        return;
    }
    int m = grid.size();
    int n = grid[0].size();
    for (const auto& dir : dirs) {
        int r = houses[idx].first + dir[0];
        int c = houses[idx].second + dir[1];
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '.') { continue; }
        if (rowUsed[r] || colUsed[c] || housesAround(grid, r, c) != 1) { continue; }
        grid[r][c] = 'F';
        rowUsed[r] = true;
        colUsed[c] = true;
        dfs(grid, houses, idx + 1, rowUsed, colUsed, limit, out);
        grid[r][c] = '.';
        rowUsed[r] = false;
        colUsed[c] = false;
    }
}

// (2) every planting (limit = how many to collect)
// core idea: backtrack house by house; each picks 1 of its <= 4 soil neighbors
// a cell is legal if its row and col are free and it touches only this house
// T: O(4^h * m * n), S: O(h) + output — h houses, <= 4 choices each, copy per answer
vector<vector<string>> allPlantings(vector<string> grid, size_t limit) {
    if (grid.empty()) { return {}; }
    int m = grid.size();
    int n = grid[0].size();
    vector<pair<int, int>> houses;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 'H') { houses.push_back({r, c}); }
        }
    }
    vector<bool> rowUsed(m, false);
    vector<bool> colUsed(n, false);
    vector<vector<string>> out;
    dfs(grid, houses, 0, rowUsed, colUsed, limit, out);
    return out;
}

// (1) one planting
// core idea: same search, stop at the first finished grid
// T: O(4^h * m * n), S: O(h)
vector<string> plantFlowers(const vector<string>& grid) {
    vector<vector<string>> found = allPlantings(grid, 1);
    if (found.empty()) { return {}; }
    return found[0];
}

void test(const string& label, const vector<string>& grid, const vector<string>& want) {
    cout << label << (plantFlowers(grid) == want ? " PASS\n" : " FAIL\n");
}

void testAll(const string& label, const vector<string>& grid, size_t want) {
    cout << label << (allPlantings(grid, 1000).size() == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-house", {"H."}, {"HF"});
    test("only-soil-is-shared", {"H.H"}, {});
    test("two-corners", {"H..", "...", "..H"}, {"H..", "F..", ".FH"});
    test("row-already-used", {"HH.", "..."}, {"HHF", "F.."});
    test("no-legal-cell-left", {"H.", "..", "H."}, {});
    test("no-house", {".."}, {".."});

    testAll("all-two-corners", {"H..", "...", "..H"}, 2);
    testAll("all-none", {"H.H"}, 0);
}

// follow-up:
// only decide yes / no -> same DFS with limit 1; return !found.empty()
// L4 version, exactly 1 flower per row and col -> then fill the free rows N-queens style
// those extra flowers -> only on cells that touch no house
// why not greedy per house -> a pick can take the row / col another house needs
// prune -> order houses by fewest legal cells first; fail early if a house has none
// existing flowers 'F' in the input -> pre-mark their rows / cols and their houses
// big grid, many houses -> bipartite matching won't do (row / col limits); stay with
//   search
