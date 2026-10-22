// LC 218 The Skyline Problem: buildings[i] = {left, right, height}, sorted by left.
// Return the skyline's key points {x, height} (left end of each horizontal segment),
// ending with a point of height 0; no two consecutive points share a height.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::priority_queue;
using std::sort;
using std::string;
using std::vector;
using std::pair;

// sweep over every edge x with a max-heap of (height, right) and lazy deletion.
// T: O(n log n), S: O(n).
vector<vector<int>> getSkyline(const vector<vector<int>>& buildings) {
    int n = buildings.size();
    vector<int> xs;   // every left and right edge, the only places the height changes
    for (const auto& b : buildings) {
        xs.push_back(b[0]);
        xs.push_back(b[1]);
    }
    sort(xs.begin(), xs.end());
    priority_queue<pair<int, int>> heap;   // (height, right) of buildings started
    vector<vector<int>> points;
    int i = 0;
    int last = 0;   // height of the skyline so far
    for (int x : xs) {
        while (i < n && buildings[i][0] <= x) {
            heap.push({buildings[i][2], buildings[i][1]});
            i++;
        }
        // a building that already ended at x no longer counts
        while (!heap.empty() && heap.top().second <= x) { heap.pop(); }
        int h = heap.empty() ? 0 : heap.top().first;
        if (h != last) {
            points.push_back({x, h});
            last = h;
        }
    }
    return points;
}

void test(const string& label, const vector<vector<int>>& buildings,
          const vector<vector<int>>& want) {
    if (getSkyline(buildings) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("one", {{1, 4, 3}}, {{1, 3}, {4, 0}});
    test("nested", {{0, 10, 2}, {2, 5, 4}}, {{0, 2}, {2, 4}, {5, 2}, {10, 0}});
    test("touching-same-height", {{0, 3, 2}, {3, 6, 2}}, {{0, 2}, {6, 0}});
    test("gap", {{0, 2, 1}, {4, 6, 3}}, {{0, 1}, {2, 0}, {4, 3}, {6, 0}});
}
