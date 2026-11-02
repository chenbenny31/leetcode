// spending[i] >= 0 is participant i's weight. Pick k distinct winners, each draw weighted
// by spending among those not yet picked (no replacement). Zero-weight participants are
// never picked, so fewer than k winners come back if fewer than k have positive weight.

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <queue>
#include <cmath>
#include <utility>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::priority_queue;
using std::pair;
using std::pow;
using std::lower_bound;
using std::greater;
using std::sort;

// straightforward: k rounds of "draw r in [1, total], find the first prefix sum >= r",
// then zero the winner's weight and rebuild. T: O(k * n), S: O(n).
vector<int> pickWinners(vector<long long> weight, int k, mt19937& rng) {
    int n = weight.size();
    vector<int> winners;
    for (int round = 0; round < k; round++) {
        // prefix[i] = weight[0] + ... + weight[i]
        vector<long long> prefix(n);
        long long total = 0;
        for (int i = 0; i < n; i++) {
            total += weight[i];
            prefix[i] = total;
        }
        if (total == 0) { break; }
        uniform_int_distribution<long long> draw(1, total);
        long long r = draw(rng);
        int i = lower_bound(prefix.begin(), prefix.end(), r) - prefix.begin();
        winners.push_back(i);
        weight[i] = 0;
    }
    return winners;
}

// improved (A-Res): key_i = u^(1 / w_i) with u uniform in (0, 1); the k largest keys
// are a weighted sample without replacement. One pass with a size-k min-heap.
// T: O(n log k), S: O(k).
vector<int> pickWinnersRes(const vector<long long>& weight, int k, mt19937& rng) {
    uniform_real_distribution<double> unit(0.0, 1.0);
    // min-heap of (key, index): the smallest kept key is dropped first
    priority_queue<pair<double, int>, vector<pair<double, int>>,
                   greater<pair<double, int>>> heap;
    int n = weight.size();
    for (int i = 0; i < n; i++) {
        if (weight[i] == 0) { continue; }
        double key = pow(unit(rng), 1.0 / weight[i]);
        heap.push({key, i});
        if ((int)heap.size() > k) { heap.pop(); }
    }
    vector<int> winners;
    while (!heap.empty()) {
        winners.push_back(heap.top().second);
        heap.pop();
    }
    sort(winners.begin(), winners.end());
    return winners;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    mt19937 rng(42);
    vector<long long> w = {1, 0, 3};
    // winners must be distinct and never the zero-weight participant
    vector<int> a = pickWinners(w, 3, rng), b = pickWinnersRes(w, 3, rng);
    sort(a.begin(), a.end());
    check(a == vector<int>{0, 2} && b == vector<int>{0, 2}, "skip-zero");
    // index 2 should win about 3/4 of single draws
    int hitsA = 0, hitsB = 0, trials = 4000;
    for (int t = 0; t < trials; t++) {
        if (pickWinners(w, 1, rng)[0] == 2) { hitsA++; }
        if (pickWinnersRes(w, 1, rng)[0] == 2) { hitsB++; }
    }
    bool near = hitsA > 0.70 * trials && hitsA < 0.80 * trials;
    check(near && hitsB > 0.70 * trials && hitsB < 0.80 * trials, "weighted");
    check(pickWinners({0, 0}, 1, rng).empty() && pickWinnersRes({0, 0}, 1, rng).empty(),
          "all-zero");
}
