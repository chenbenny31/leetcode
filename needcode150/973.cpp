// max heap of size k, T: O(nlogk), S: O(k)

#include <vector>
#include <queue> // std::priority_queue
#include <functional> // std::greater

class Solution {
public:
    std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
        // max-heap, evict farthest when size exceeds k
        auto cmp = [](const std::vector<int>& a, const std::vector<int>& b) {
            return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
        };

        std::vector<std::vector<int>> buf;
        buf.reserve(k + 1);

        std::priority_queue<std::vector<int>,
                            std::vector<std::vector<int>>,
                            decltype(cmp)> heap(cmp, std::move(buf)); // max-heap

        for (auto& p : points) {
            heap.push(p);
            if (static_cast<int>(heap.size()) > k) { heap.pop(); }
        }

        std::vector<std::vector<int>> res;
        res.reserve(k);
        while (!heap.empty()) {
            res.push_back(heap.top());
            heap.pop();
        }
        return res;
    }
};

// sort, T: O(nlogk), S: O(1)

#include <vector>
#include <algorithm> // std::sort, std::partail_sort (internally use heap)

class Solution {
public:
    std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
        std::partial_sort(points.begin(), points.begin() + k, points.end(),
                          [](const std::vector<int>& a, const std::vector<int>& b) {
                              return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
                          });
        return std::vector<std::vector<int>>(points.begin(), points.begin() + k);
    }
};

// quick-select, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::swap, std::shffle
#include <random> // std::mt19937, std::random_device
#include <utility> // std::pair

class Solution {
public:
    std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k) {
        std::mt19937 rng(std::random_device{}());
        std::shuffle(points.begin(), points.end(), rng);

        int lo = 0, hi = static_cast<int>(points.size()) - 1;
        while (lo < hi) {
            auto [lt, gt] = partition(points, lo, hi);
            if (lt > k - 1) { hi = lt - 1; }
            else if (gt < k - 1) { lo = gt + 1; }
            else break;
        }
        return std::vector<std::vector<int>>(points.begin(), points.begin() + k);
    }

private:
    int dist(const std::vector<int>& p) { return p[0]*p[0] + p[1]*p[1]; }

    std::pair<int, int> partition(std::vector<std::vector<int>>& points, int lo, int hi) {
        int pvt_dist = dist(points[lo]);
        int lt = lo, gt = hi, i = lo;
        while (i <= gt) {
            int d = dist(points[i]);
            if (d < pvt_dist) { std::swap(points[i++], points[lt++]); }
            else if (d > pvt_dist) { std::swap(points[i], points[gt--]); }
            else { ++i; }
        }
        return {lt, gt};
    }
};

// dist not sqrt: avoid floting point entirely
// quick-select shuffle: mandatory, otherwise adversarial input hits O(n2), std::mt19937 guarantees O(n) expected
