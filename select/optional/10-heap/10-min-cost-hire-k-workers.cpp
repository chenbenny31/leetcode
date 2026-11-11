// LC 857 Minimum Cost to Hire K Workers (Hard). Pattern: sort by ratio + max-heap.
// Hire exactly k workers; pay is proportional to quality within the group and each
// worker gets at least their wage. Return the minimum total pay.

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::fabs;
using std::min;
using std::pair;
using std::priority_queue;
using std::sort;
using std::string;
using std::vector;

// The group's rate is its largest wage / quality. Fix it at each worker in ratio order;
// cost = ratio * sum of qualities, so keep the k smallest qualities.
// T: O(n log n), S: O(n).
double mincostToHireWorkers(const vector<int>& quality, const vector<int>& wage, int k) {
    int n = quality.size();
    // (wage / quality, quality), lowest ratio first after sorting
    vector<pair<double, int>> workers;
    for (int i = 0; i < n; i++) {
        workers.push_back({(double)wage[i] / quality[i], quality[i]});
    }
    sort(workers.begin(), workers.end());
    // qualities in the group; top = largest, dropped first
    priority_queue<int> heap;
    // sum of qualities in the heap
    int sumQuality = 0;
    double best = 1e18;
    for (auto [ratio, qual] : workers) {
        heap.push(qual);
        sumQuality += qual;
        if ((int)heap.size() > k) {
            sumQuality -= heap.top();
            heap.pop();
        }
        // ratio is the largest so far, so everyone is paid at least their wage
        if ((int)heap.size() == k) { best = min(best, ratio * sumQuality); }
    }
    return best;
}

void test(const string& label, const vector<int>& quality, const vector<int>& wage, int k,
          double want) {
    bool ok = fabs(mincostToHireWorkers(quality, wage, k) - want) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k-one", {2, 5}, {8, 10}, 1, 8.0);
    test("hire-all", {1, 2, 3}, {3, 2, 3}, 3, 18.0);
    test("drop-big-quality", {1, 10, 1}, {1, 10, 3}, 2, 6.0);
    test("fraction", {3, 2}, {4, 1}, 2, 20.0 / 3);
}

// follow-up: why the heap pops the largest quality -> with the rate fixed, the cheapest
//   k workers are the k smallest qualities among those with ratio <= rate
// follow-up: avoid floating point in the sort -> compare ratios by cross-multiplying,
//   w1 * q2 < w2 * q1
