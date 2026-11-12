// LC 528 Random Pick with Weight (Google-tagged; also reported as "random pick with
// weight using recorded draws"). Given positive weights w[0..n-1], pickIndex() returns i
// with probability w[i] / total, total = sum of w (fits in int: n <= 1e4, w[i] <= 1e5).
// pickFor(r) maps one draw r in [0, total) to its index, so tests use fixed draws.
// Recorded draws: pickAll(draws) maps each recorded draw (in [0, total)) to an index.

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::upper_bound;
using std::rand;
using std::srand;
using std::abs;

// the weights lie end to end on [0, total): index i owns [prefix[i - 1], prefix[i]).
// T: build O(n), pickForLinear O(n), pickFor / pickIndex O(log n), pickAll O(D log n)
// for D draws. S: O(n).
class WeightedPicker {
    // weights[i] = w[i]
    vector<int> weights;
    // prefixWeights[i] = sum of w[0..i]
    vector<int> prefixWeights;

public:
    WeightedPicker(const vector<int>& w) : weights(w) {
        int running = 0;
        for (int weight : w) {
            running += weight;
            prefixWeights.push_back(running);
        }
    }

    // straightforward: accumulate weights until the running sum passes r.
    int pickForLinear(int r) {
        int running = 0;
        int n = weights.size();
        for (int i = 0; i < n; i++) {
            running += weights[i];
            if (running > r) { return i; }
        }
        return n - 1;
    }

    // improved: binary search the prefix sums.
    int pickFor(int r) {
        // r belongs to the first i with prefix > r (prefix == r is where i + 1 starts)
        auto it = upper_bound(prefixWeights.begin(), prefixWeights.end(), r);
        return it - prefixWeights.begin();
    }

    int pickIndex() { return pickFor(rand() % prefixWeights.back()); }

    // recorded draws: the same lookup for each draw.
    vector<int> pickAll(const vector<int>& draws) {
        vector<int> picks;
        for (int r : draws) { picks.push_back(pickFor(r)); }
        return picks;
    }
};

void test(const string& label, const vector<int>& w, const vector<int>& draws,
          const vector<int>& want) {
    WeightedPicker picker(w);
    bool ok = picker.pickAll(draws) == want;
    for (int d = 0; d < (int)draws.size(); d++) {
        ok = ok && picker.pickForLinear(draws[d]) == want[d];
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

// wantShare[i] = expected fraction of pickIndex() calls that return i
void testSample(const string& label, const vector<int>& w,
                const map<int, double>& wantShare) {
    WeightedPicker picker(w);
    srand(7);
    int calls = 4000;
    // seen[i] = times pickIndex returned i
    map<int, int> seen;
    for (int c = 0; c < calls; c++) { seen[picker.pickIndex()]++; }
    bool ok = seen.size() == wantShare.size();
    for (const auto& [index, share] : wantShare) {
        ok = ok && abs(1.0 * seen[index] / calls - share) < 0.05;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-to-three", {1, 3}, {0, 1, 3}, {0, 1, 1});
    test("boundaries", {2, 2, 2}, {1, 2, 3, 4, 5}, {0, 1, 1, 2, 2});
    test("single", {5}, {0, 4}, {0, 0});
    test("heavy-middle", {1, 10, 1}, {0, 1, 10, 11}, {0, 1, 1, 2});
    testSample("sample-1-to-3", {1, 3}, {{0, 0.25}, {1, 0.75}});
}

// follow-up: recorded draws sorted -> one merge-like pass with a pointer into
//   prefixWeights, O(n + D) instead of O(D log n)
// follow-up: weights updated between picks -> Fenwick tree over w: update O(log n);
//   pick = descend the tree for the first index whose prefix sum > r, O(log n)
// follow-up: O(1) per pick -> Walker / Vose alias method: O(n) build of n columns of
//   height total / n, each split between two indices; a pick = one column + one coin
// follow-up: draw is a double u in [0, 1) -> r = u * total, compare against prefix sums
//   as doubles (same upper_bound)
