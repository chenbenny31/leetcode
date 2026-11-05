// ByteDance / LC 939: distinct points with 0 <= x, y <= 40000; return the min area of a
// rectangle with sides parallel to the axes and all 4 corners in the set, else 0.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::abs;

// Each pair as a diagonal; look up the other two corners. T: O(n^2), S: O(n).
int minAreaRect(const vector<vector<int>>& points) {
    // x * 40001 + y of every point (fits in int for coords <= 40000)
    unordered_set<int> pts;
    for (const vector<int>& p : points) { pts.insert(p[0] * 40001 + p[1]); }
    int n = points.size();
    // 0 = no rectangle found yet
    int best = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int x1 = points[i][0];
            int y1 = points[i][1];
            int x2 = points[j][0];
            int y2 = points[j][1];
            // same row or column cannot be a diagonal
            if (x1 == x2 || y1 == y2) { continue; }
            if (!pts.count(x1 * 40001 + y2) || !pts.count(x2 * 40001 + y1)) { continue; }
            int area = abs(x1 - x2) * abs(y1 - y2);
            if (best == 0 || area < best) { best = area; }
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& points, int want) {
    cout << label << (minAreaRect(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {{1, 1}, {1, 3}, {3, 1}, {3, 3}, {2, 2}}, 4);
    test("lc-ex2", {{1, 1}, {1, 3}, {3, 1}, {3, 3}, {4, 1}, {4, 3}}, 2);
    test("none", {{1, 1}, {1, 3}, {3, 1}}, 0);
}

// follow-up: group by column -> map<x, sorted ys>; for x ascending, each y pair (y1, y2):
//   if seen, area = (x - lastX[{y1, y2}]) * (y2 - y1); then lastX[{y1, y2}] = x
// follow-up: any orientation (LC 963) -> bucket pairs by (x1 + x2, y1 + y2, len^2) as
//   diagonals; two pairs a-c, b-d in a bucket form a rectangle, area = |ab| * |ad|
// follow-up: count rectangles -> per column y pair, ans += cnt[{y1, y2}]++
