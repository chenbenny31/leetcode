// quick-select

#include <vector>
#include <algorithm> // std::nth_element

class Solution {
public:
    std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
        auto closer = [](const std::vector<int>& a, const std::vector<int>& b) {
           return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1]; 
        };
        std::nth_element(points.begin(), points.begin() + k, points.end(), closer);
        return std::vector<std::vector<int>>(points.begin(), points.begin() + k);
    }
};

// heap

#include <queue>
#include <utility>
#include <functional>

class Solution {
public:
    std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
        std::vector<std::pair<int, int>> buf;
        buf.reserve(k + 1);
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                            std::less<std::pair<int, int>>> maxHeap(std::less<std::pair<int, int>>{}, std::move(buf));

        for (int i = 0; i < static_cast<int>(points.size()); i++) {
            int x = points[i][0];
            int y = points[i][1];
            maxHeap.push({ x*x + y*y, i });
            if (static_cast<int>(maxHeap.size()) > k) { maxHeap.pop(); }
        }

        std::vector<std::vector<int>> out;
        out.reserve(k);
        while (!maxHeap.empty()) {
            out.push_back(points[maxHeap.top().second]);
            maxHeap.pop();
        }
        return out;
    }
};
