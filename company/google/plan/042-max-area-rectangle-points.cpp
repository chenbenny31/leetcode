// Max-Area Rectangle From Points (Google, interview report; LC 963 turned into max)
// points w/ int coordinates; pick 4 as the corners of a rectangle, any orientation
// return the largest area, 0 if none
// reported: x2 (L4 onsite; asked to improve on O(n^3))
//
// clarify:
// dup points -> assumed none
// axis-parallel only? -> no, any orientation
// degenerate (zero area) -> not a rectangle
// return -> area as double
// n -> up to a few thousand; O(n^2) pairs is the target

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::sqrt;
using std::fabs;
using std::max;

// distance between points a and b
double dist(const vector<int>& a, const vector<int>& b) {
    double dx = a[0] - b[0];
    double dy = a[1] - b[1];
    return sqrt(dx * dx + dy * dy);
}

// core idea: the 2 diagonals of a rectangle share midpoint and len
// bucket point pairs by (x sum, y sum, len^2); any 2 pairs in a bucket = a rectangle
// T: O(n^2 log n) to bucket + pairs inside a bucket, S: O(n^2)
double maxRectangleArea(const vector<vector<int>>& points) {
    int n = points.size();
    // groups[{x sum, y sum, len^2}] = point pairs (i, j) w/ that midpoint * 2 and len^2
    map<vector<long long>, vector<pair<int, int>>> groups;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long dx = points[i][0] - points[j][0];
            long long dy = points[i][1] - points[j][1];
            long long sumX = points[i][0] + points[j][0];
            long long sumY = points[i][1] + points[j][1];
            groups[{sumX, sumY, dx * dx + dy * dy}].push_back({i, j});
        }
    }
    double result = 0;
    for (const auto& group : groups) {
        const vector<pair<int, int>>& pairs = group.second;
        for (size_t a = 0; a < pairs.size(); a++) {
            for (size_t b = a + 1; b < pairs.size(); b++) {
                // diagonals (p, _) and (q1, q2) -> sides p-q1 and p-q2
                const vector<int>& p = points[pairs[a].first];
                double side1 = dist(p, points[pairs[b].first]);
                double side2 = dist(p, points[pairs[b].second]);
                result = max(result, side1 * side2);
            }
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& points, double want) {
    bool ok = fabs(maxRectangleArea(points) - want) < 1e-6;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("axis-square", {{0, 0}, {0, 2}, {2, 0}, {2, 2}}, 4);
    test("tilted-square", {{1, 0}, {0, 1}, {1, 2}, {2, 1}}, 2);
    test("no-rectangle", {{0, 0}, {1, 0}, {0, 1}}, 0);
    test("pick-larger", {{0, 0}, {0, 1}, {1, 0}, {1, 1}, {5, 5}, {5, 8}, {7, 5}, {7, 8}},
         6);
    test("collinear", {{0, 0}, {1, 1}, {2, 2}, {3, 3}}, 0);
}

// follow-up:
// the O(n^3) version to state first -> 3 points w/ a right angle, 4th = p1 + p2 - p0
// why sums, not the midpoint -> stays in ints, no float keys
// worst case -> many points on one circle fill one bucket: O(n^3) again
// that bucket in O(k log k) -> area = len^2 / 2 * sin(angle); sort by angle, 2 pointers
// min area instead (LC 963) -> same buckets, take the min positive area
// axis-parallel only (LC 939) -> set of points; each diagonal pair checks the 2 others
// count rectangles -> sum of k * (k - 1) / 2 over buckets
// squares only -> also need equal sides: side1 == side2
