// A tower at the origin with height r covers every house whose distance to the origin is
// strictly less than r. Each covered house pays price; the tower costs heightCost per
// unit of height. Return the max profit (0 = build no tower).
// Real height: "strictly less" makes the best profit a supremum, approached as r shrinks
// to the k-th distance. Follow-up: integer height.

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sqrt;
using std::fabs;
using std::sort;
using std::max;

// Covering the k nearest houses needs height just above the k-th smallest distance, so
// sort distances and try every k. T: O(n log n), S: O(n).
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

// Follow-up: integer height. The smallest integer H with H > distance is the smallest H
// with H * H > squared distance; use integers so sqrt rounding cannot pick the wrong H.
// T: O(n log n), S: O(n).
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
        // fix sqrt rounding: smallest height with height^2 > squared distance
        while (height * height > squared[i]) { height--; }
        while (height * height <= squared[i]) { height++; }
        long long count = i + 1;
        long long profit = price * count - heightCost * height;
        best = max(best, profit);
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& houses, int price,
          int heightCost, double want, long long wantInt) {
    bool ok = fabs(maxProfit(houses, price, heightCost) - want) < 1e-9;
    bool okInt = maxProfitIntHeight(houses, price, heightCost) == wantInt;
    cout << label << (ok && okInt ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cover-all", {{0, 1}, {3, 4}, {6, 8}}, 10, 1, 20.0, 19);
    test("cover-nearest", {{0, 1}, {3, 4}, {6, 8}}, 10, 3, 7.0, 4);
    test("no-tower", {{3, 4}}, 10, 20, 0.0, 0);
    test("same-distance", {{3, 4}, {4, 3}, {0, 5}}, 10, 4, 10.0, 6);
}

// follow-up: covered when distance <= r -> same loop, the profit is exact (no supremum)
// follow-up: many (price, heightCost) queries -> profit(k) = price * k - cost * d_k is a
//   line per k; convex hull trick over the sorted distances, O(log n) per query
