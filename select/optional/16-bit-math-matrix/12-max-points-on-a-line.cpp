// LC 149 Max Points on a Line (Hard) — geometry with hashing (normalized slope)
// points[i] = {x, y}; return max num of points on one straight line
//
// clarify:
// dup points -> LC: all distinct; else follow-up
// coordinate range -> dy, dx must fit in int
// vertical / horizontal lines -> covered by reduced (dy, dx), no division
// n size -> small enough for O(n^2)

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

// Euclid; gcd(a, 0) = a
int gcd(int a, int b) {
    while (b != 0) {
        int rest = a % b;
        a = b;
        b = rest;
    }
    return a;
}

// core idea: fix an anchor; points on one line through it share a slope
// cnt slopes as reduced (dy, dx) pairs w/ one fixed sign
// T: O(n^2 log n), S: O(n)
int maxPoints(const vector<vector<int>>& points) {
    int n = points.size();
    int best = n == 0 ? 0 : 1;
    for (int anchor = 0; anchor < n; anchor++) {
        // count[reduced (dy, dx) from anchor] = points seen in that direction
        map<pair<int, int>, int> count;
        // other > anchor is enough: first point of best line sees the rest after it
        for (int other = anchor + 1; other < n; other++) {
            int dy = points[other][1] - points[anchor][1];
            int dx = points[other][0] - points[anchor][0];
            int common = gcd(abs(dy), abs(dx));
            dy /= common;
            dx /= common;
            // one sign per line: dx > 0, or vertical w/ dy > 0
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

// follow-up:
// all points collinear? (Google) -> first two distinct a, b; all (b - a) x (p - a) == 0
// dup points -> cnt copies of the anchor (dy == dx == 0), add to every slope's cnt
// why not double slopes -> dy / dx rounds; reduced int pair is exact, covers vertical
// detect squares (LC 2013) -> cnt map; each point on query's x fixes the 2 other corners
// max-area rectangle (Google) -> group point pairs by (midpoint, len) of the diagonal
// min area rectangle, axis-aligned (LC 939) -> set; each diagonal pair, check 2 corners
