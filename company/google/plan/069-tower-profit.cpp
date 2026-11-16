// Tower Profit (Google, interview report)
// (1) tower at origin w/ height r covers houses at distance < r; covered house pays price
// (1) tower costs heightCost per unit of height; return max profit (0 = no tower); real r
// (2) integer height
// (3) many (price, heightCost) queries on the same houses
//
// clarify:
// covered -> strict < (vs <=); best is a supremum as r shrinks to the k-th distance
// same distance / house at origin -> ties are covered together
// coordinates up to 1e9 -> squares need long long
// n up to 1e5 -> O(n log n)
// return -> profit only

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::sqrt;
using std::fabs;
using std::sort;
using std::max;
using std::pair;

// (1) real height
// core idea: covering the k nearest needs height just above d_k (k-th smallest distance)
// sort distances, try every k: profit(k) = price * k - heightCost * d_k
// T: O(n log n), S: O(n)
double maxProfit(const vector<vector<int>>& houses, int price, int heightCost) {
    // distance of each house to the origin, ascending
    vector<double> distances;
    for (const vector<int>& house : houses) {
        double x = house[0];
        double y = house[1];
        distances.push_back(sqrt(x * x + y * y));
    }
    sort(distances.begin(), distances.end());

    double best = 0;
    int n = distances.size();
    for (int i = 0; i < n; i++) {
        int count = i + 1;
        // height just above distances[i] covers the first count houses
        double profit = (double)price * count - (double)heightCost * distances[i];
        best = max(best, profit);
    }
    return best;
}

// (2) integer height
// core idea: smallest int H above the distance = smallest H w/ H * H > squared distance
// stay on integers so sqrt rounding cannot pick the wrong H
// T: O(n log n), S: O(n)
long long maxProfitIntHeight(const vector<vector<int>>& houses, int price,
                             int heightCost) {
    // squared distance of each house, ascending
    vector<long long> squared;
    for (const vector<int>& house : houses) {
        long long x = house[0];
        long long y = house[1];
        squared.push_back(x * x + y * y);
    }
    sort(squared.begin(), squared.end());

    long long best = 0;
    int n = squared.size();
    for (int i = 0; i < n; i++) {
        long long height = (long long)sqrt((double)squared[i]);
        // fix sqrt rounding: smallest height w/ height^2 > squared distance
        while (height * height > squared[i]) { height--; }
        while (height * height <= squared[i]) { height++; }
        long long count = i + 1;
        long long profit = price * count - heightCost * height;
        best = max(best, profit);
    }
    return best;
}

// (3) many queries
// core idea: profit(k) peaks at a vertex of the lower hull of (k, d_k), k = 0..n, d_0 = 0
// along the hull the gain per edge only falls -> binary search the first edge w/o gain
// T: O(n log n + q log n), S: O(n)
vector<double> maxProfitQueries(const vector<vector<int>>& houses,
                                const vector<vector<int>>& queries) {
    // distance of each house to the origin, ascending
    vector<double> distances;
    for (const vector<int>& house : houses) {
        double x = house[0];
        double y = house[1];
        distances.push_back(sqrt(x * x + y * y));
    }
    sort(distances.begin(), distances.end());

    // lower hull of (count, distance of the count-th house), counts increasing
    vector<pair<int, double>> hull = {{0, 0.0}};
    int n = distances.size();
    for (int i = 0; i < n; i++) {
        int count = i + 1;
        double d = distances[i];
        while (hull.size() >= 2) {
            auto [count1, d1] = hull[hull.size() - 2];
            auto [count2, d2] = hull.back();
            // keep the last vertex only if the slope rises there (cross-multiplied)
            if ((d2 - d1) * (count - count2) < (d - d2) * (count2 - count1)) { break; }
            hull.pop_back();
        }
        hull.push_back({count, d});
    }

    vector<double> answers;
    for (const vector<int>& query : queries) {
        double price = query[0];
        double heightCost = query[1];
        // first edge hull[e] -> hull[e + 1] that does not raise profit; e = best vertex
        // e = last vertex when every edge gains
        int lo = 0;
        int hi = hull.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            double gain = price * (hull[mid + 1].first - hull[mid].first)
                          - heightCost * (hull[mid + 1].second - hull[mid].second);
            if (gain > 0) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        answers.push_back(price * hull[lo].first - heightCost * hull[lo].second);
    }
    return answers;
}

void test(const string& label, const vector<vector<int>>& houses, int price,
          int heightCost, double want, long long wantInt) {
    bool ok = fabs(maxProfit(houses, price, heightCost) - want) < 1e-9;
    bool okInt = maxProfitIntHeight(houses, price, heightCost) == wantInt;
    cout << label << (ok && okInt ? " PASS\n" : " FAIL\n");
}

void testQueries(const string& label, const vector<vector<int>>& houses,
                 const vector<vector<int>>& queries, const vector<double>& want) {
    vector<double> got = maxProfitQueries(houses, queries);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        ok = fabs(got[i] - want[i]) < 1e-9;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cover-all", {{0, 1}, {3, 4}, {6, 8}}, 10, 1, 20.0, 19);
    test("no-tower", {{3, 4}}, 10, 20, 0.0, 0);
    test("same-distance", {{3, 4}, {4, 3}, {0, 5}}, 10, 4, 10.0, 6);
    test("dip-then-rise", {{0, 1}, {0, 6}, {0, 7}, {0, 8}}, 3, 1, 4.0, 3);
    testQueries("queries-hull-skip", {{0, 1}, {3, 4}, {4, 3}}, {{3, 1}, {10, 1}, {1, 2}},
                {4.0, 25.0, 0.0});
    testQueries("queries-match-loop", {{0, 1}, {3, 4}, {6, 8}}, {{10, 1}, {10, 3}},
                {20.0, 7.0});
    testQueries("queries-free-height", {{1, 1}, {2, 2}}, {{3, 0}}, {6.0});
    testQueries("queries-no-houses", {}, {{5, 1}}, {0.0});
}

// follow-up:
// covered when distance <= r -> profit is exact (no supremum); int H: H * H >= squared
// return the height too -> keep bestHeight = distances[i] (or height) when best improves
// stop at the first drop in profit -> wrong: profit(k) is not unimodal; try every k
// (3) binary search on raw points -> wrong: raw gains are not monotonic, only hull edges
