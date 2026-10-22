// LC 973 K Closest Points to Origin: return the k points [x, y] closest to (0, 0) by
// Euclidean distance, in any order.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::sort;

// Max-heap capped at k: for the k smallest we evict the largest, so the farthest kept
// point sits on top. Squared distance avoids sqrt. T: O(n log k), S: O(k).
vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
    int n = points.size();
    priority_queue<pair<int, int>> heap;   // (squared distance, point index), k nearest
    for (int i = 0; i < n; i++) {
        heap.push({points[i][0] * points[i][0] + points[i][1] * points[i][1], i});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<vector<int>> res;
    while (!heap.empty()) {
        res.push_back(points[heap.top().second]);
        heap.pop();
    }
    return res;
}

void test(const string& label, vector<vector<int>> points, int k,
          vector<vector<int>> want) {
    vector<vector<int>> got = kClosest(points, k);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-nearest", {{3, 3}, {1, 0}, {-2, 2}, {5, 5}}, 2, {{-2, 2}, {1, 0}});
    test("all", {{1, 1}, {0, 2}}, 2, {{0, 2}, {1, 1}});
    test("origin", {{0, 0}, {-1, -1}}, 1, {{0, 0}});
}
