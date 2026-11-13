// LC 149 Max Points on a Line (Hard) — geometry with hashing (normalized slope)
// points[i] = {x, y}. Return the largest number of points that lie on one straight line.
// Clarify: duplicate points (LC: all distinct); coordinate range (overflow of the
//   differences); vertical and horizontal lines; n small enough for O(n^2).

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::max;
using std::abs;

int gcd(int a, int b) {
    while (b != 0) {
        int rest = a % b;
        a = b;
        b = rest;
    }
    return a;
}

// Core idea: fix an anchor; two other points share a line through it exactly when their
// slopes match, so count the slopes as reduced (dy, dx) pairs with one fixed sign.
// T: O(n^2 log n), S: O(n).
int maxPoints(const vector<vector<int>>& points) {
    int n = points.size();
    int best = n == 0 ? 0 : 1;
    for (int anchor = 0; anchor < n; anchor++) {
        // reduced (dy, dx) from the anchor -> points seen in that direction
        map<pair<int, int>, int> count;
        // the first point of the best line sees all its other points after it
        for (int other = anchor + 1; other < n; other++) {
            int dy = points[other][1] - points[anchor][1];
            int dx = points[other][0] - points[anchor][0];
            int common = gcd(abs(dy), abs(dx));
            dy /= common;
            dx /= common;
            // one sign per line: dx > 0, or vertical with dy > 0
            if (dx < 0 || (dx == 0 && dy < 0)) {
                dy = -dy;
                dx = -dx;
            }
            count[{dy, dx}]++;
            best = max(best, count[{dy, dx}] + 1);
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& points, int want) {
    cout << label << (maxPoints(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("axis-lines", {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {2, 0}}, 4);
    test("opposite-sides", {{1, 1}, {0, 0}, {2, 2}, {3, 1}}, 3);
    test("reduced-slope", {{0, 0}, {2, 4}, {3, 6}, {1, 3}}, 3);
    test("single-point", {{5, 5}}, 1);
    return 0;
}

// follow-up (ideas only):
// - are all points collinear (Google) -> take the first two distinct points; every other
//   point must have cross product (b - a) x (p - a) == 0; O(n), no division
// - duplicate points -> count the copies of the anchor separately (dy == dx == 0) and
//   add them to every slope's count
// - why not double slopes -> dy / dx rounds, so equal slopes can differ and different
//   ones can collide; the reduced integer pair is exact (and covers vertical lines)
// - detect squares (LC 2013) -> count map of points; for each stored point on the same x
//   as the query, the side length fixes the two other corners (left and right)
// - max-area rectangle from points (Google) -> group point pairs by (midpoint, length)
//   of the diagonal; two pairs in one group form a rectangle
// - min area rectangle, axis-aligned (LC 939) -> set of points; each pair with different
//   x and y is a diagonal, check that the other two corners exist
