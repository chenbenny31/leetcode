// LC 149 Max Points on a Line: given distinct points on the plane, return the maximum
// number of points that lie on one straight line.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::gcd;
using std::max;

// Fix point i; every later point j defines a direction from i. Points sharing the same
// reduced direction are collinear with i (no floating point slopes).
// T: O(n^2 log n), S: O(n).
int maxPoints(vector<vector<int>>& points) {
    int n = points.size();
    int best = 1;
    for (int i = 0; i < n; i++) {
        // reduced direction from point i -> later points in that direction
        map<pair<int, int>, int> cnt;
        for (int j = i + 1; j < n; j++) {
            int dx = points[j][0] - points[i][0];
            int dy = points[j][1] - points[i][1];
            int g = gcd(dx, dy);
            dx /= g;
            dy /= g;
            // opposite directions are the same line
            if (dx < 0 || (dx == 0 && dy < 0)) {
                dx = -dx;
                dy = -dy;
            }
            cnt[{dx, dy}]++;
            best = max(best, cnt[{dx, dy}] + 1);
        }
    }
    return best;
}

void test(const string& label, vector<vector<int>> points, int want) {
    cout << label << (maxPoints(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diagonal", {{1, 1}, {2, 2}, {5, 0}, {3, 3}}, 3);
    test("vertical", {{0, 0}, {0, 1}, {1, 5}, {0, 2}}, 3);
    test("opposite-sides", {{0, 0}, {1, 1}, {-1, -1}}, 3);
    test("single", {{4, 4}}, 1);
}
