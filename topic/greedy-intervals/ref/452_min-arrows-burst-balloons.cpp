// LC 452 Minimum Number of Arrows to Burst Balloons: balloon i spans x in
// [points[i][0], points[i][1]]. An arrow at x bursts every balloon covering x.
// Return the minimum arrows to burst all balloons.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::string;
using std::vector;
using std::sort;

// Sort by end and shoot at the end of the first unburst balloon: it bursts everything
// that starts at or before it. T: O(n log n), S: O(1).
int findMinArrowShots(vector<vector<int>> points) {
    sort(points.begin(), points.end(),
         [](const vector<int>& a, const vector<int>& b) { return a[1] < b[1]; });
    int arrows = 0;
    long long shot = LLONG_MIN;   // x of the latest arrow
    for (const auto& p : points) {
        if (p[0] > shot) {
            arrows++;
            shot = p[1];
        }
    }
    return arrows;
}

void test(const string& label, const vector<vector<int>>& points, int want) {
    cout << label << (findMinArrowShots(points) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-shared", {{1, 3}, {2, 4}, {5, 6}}, 2);
    test("touching", {{1, 2}, {2, 3}}, 1);
    test("extreme-values", {{-2147483648, 2147483647}, {-2147483648, -2147483648}}, 1);
    test("empty", {}, 0);
}
