// LC 2271: tiles[i] = [l, r] means cells l..r are white; tiles don't overlap. A carpet
// covers carpetLen consecutive cells. Return the max number of white cells it can cover.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::max;
using std::min;

// straightforward: an optimal carpet starts at some tile's l; for each start, add up the
// covered part of every tile. T: O(n^2), S: O(1).
long long maxCovered(const vector<vector<int>>& tiles, int len) {
    long long best = 0;
    for (const auto& t : tiles) {
        long long end = (long long)t[0] + len - 1, covered = 0;
        for (const auto& u : tiles) {
            long long lo = max<long long>(u[0], t[0]), hi = min<long long>(u[1], end);
            if (lo <= hi) { covered += hi - lo + 1; }
        }
        best = max(best, covered);
    }
    return best;
}

// improved: sort; two pointers where tiles[i..j) lie fully under the carpet and tile j
// may be partly covered. T: O(n log n), S: O(1) extra.
long long maxCoveredWindow(vector<vector<int>> tiles, int len) {
    sort(tiles.begin(), tiles.end());
    int n = tiles.size(), j = 0;
    long long best = 0, full = 0;
    for (int i = 0; i < n; i++) {
        long long end = (long long)tiles[i][0] + len - 1;
        while (j < n && tiles[j][1] <= end) {
            full += tiles[j][1] - tiles[j][0] + 1;
            j++;
        }
        long long part = 0;
        if (j < n && tiles[j][0] <= end) { part = end - tiles[j][0] + 1; }
        best = max(best, full + part);
        if (j > i) {
            full -= tiles[i][1] - tiles[i][0] + 1;
        } else {
            j = i + 1;
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& tiles, int len,
          long long want) {
    bool ok = maxCovered(tiles, len) == want && maxCoveredWindow(tiles, len) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gap", {{1, 2}, {4, 6}}, 4, 3);
    test("inside", {{1, 10}}, 3, 3);
    test("two-full", {{1, 2}, {3, 4}}, 4, 4);
    test("unsorted", {{10, 11}, {1, 1}}, 2, 2);
}
