// LC 947 Most Stones Removed with Same Row or Column: stones[i] = [x, y] on a grid, at
// most one per cell. A stone can be removed if another stone shares its row or column.
// Return the maximum number of stones that can be removed.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

int findRoot(vector<int>& uf, int x) {
    while (uf[x] != x) {
        uf[x] = uf[uf[x]];
        x = uf[x];
    }
    return x;
}

// Each connected group of stones can shrink to one stone, so the answer is
// stones - groups. Union a stone's row with its column. T: O(N * alpha), S: O(1).
int removeStones(vector<vector<int>>& stones) {
    // rows are nodes 0..10000, columns are nodes 10001..20001
    vector<int> uf(20002);
    for (int i = 0; i < 20002; i++) { uf[i] = i; }
    for (const auto& s : stones) {
        int rx = findRoot(uf, s[0]), ry = findRoot(uf, s[1] + 10001);
        uf[rx] = ry;
    }
    unordered_set<int> groups;   // roots of the groups that hold at least one stone
    for (const auto& s : stones) { groups.insert(findRoot(uf, s[0])); }
    return stones.size() - groups.size();
}

void test(const string& label, vector<vector<int>> stones, int want) {
    cout << label << (removeStones(stones) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-group-chain", {{0, 0}, {0, 1}, {1, 1}, {1, 2}}, 3);
    test("isolated-stones", {{0, 0}, {1, 1}, {2, 2}}, 0);
    test("two-groups", {{0, 0}, {0, 1}, {5, 5}, {5, 6}}, 2);
    test("single", {{3, 4}}, 0);
}
