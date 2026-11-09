// LC 973 K Closest Points to Origin (Medium). Pattern: size-k max-heap.
// Return the k points [x, y] closest to (0, 0) by Euclidean distance, in any order.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::sort;

// Keep the k nearest in a max-heap so the farthest kept point is evicted first;
// squared distance avoids sqrt. T: O(n log k), S: O(k).
vector<vector<int>> kClosest(const vector<vector<int>>& points, int k) {
    int n = points.size();
    // (squared distance, point index) of the k nearest so far, farthest on top
    priority_queue<pair<int, int>> heap;
    for (int i = 0; i < n; i++) {
        int x = points[i][0];
        int y = points[i][1];
        heap.push({x * x + y * y, i});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<vector<int>> result;
    while (!heap.empty()) {
        result.push_back(points[heap.top().second]);
        heap.pop();
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& points, int k,
          const vector<vector<int>>& want) {
    vector<vector<int>> got = kClosest(points, k);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-nearest", {{3, 3}, {1, 0}, {-2, 2}, {5, 5}}, 2, {{-2, 2}, {1, 0}});
    test("all", {{1, 1}, {0, 2}}, 2, {{0, 2}, {1, 1}});
    test("origin", {{0, 0}, {-1, -1}}, 1, {{0, 0}});
}

// follow-up: O(n) average -> quickselect on squared distance (same as LC 215), then
//   return the first k
