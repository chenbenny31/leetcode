// LC 528 Random Pick with Weight (Google-tagged; also reported as "random pick with
// weight using recorded draws"). Given positive weights w[0..n-1], pickIndex() returns i
// with probability w[i] / total, total = sum of w (fits in int: n <= 1e4, w[i] <= 1e5).
// pickFor(r) maps one draw r in [0, total) to its index, so tests use fixed draws.
// Recorded draws: pickAll(draws) maps each recorded draw (in [0, total)) to an index.
//
// Clarify:
// - weights positive? -> yes; a zero weight (after an update) must never be picked
// - up to 1e4 picks -> O(n) build once, O(log n) per pick
// - total fits in int here (<= 1e9); bigger -> long long prefix sums
// - return a 0-based index; draws are integers in [0, total)
// Idea: lay the weights end to end on [0, total): index i owns the stretch
//   [prefix[i] - w[i], prefix[i]). A uniform r lands in it with probability w[i] / total,
//   and its owner is the first prefix > r, found by binary search.
// Pitfalls:
// - upper_bound(r), not lower_bound(r): r == prefix[i] already belongs to i + 1
// - draw r in [0, total); a draw in [1, total] needs lower_bound instead
// - rand() % total is slightly biased and RAND_MAX may be 32767; in real code use
//   mt19937 with uniform_int_distribution<int>(0, total - 1)
// Tests to write: weights 1 and 3; draws on every boundary; a single weight; a heavy
//   middle; sampled shares close to w / total.

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

// follow-up: update(i, weight) between picks. A Fenwick tree over w keeps prefix sums in
// O(log n); pickFor walks down the tree to the last position whose prefix sum is <= r,
// and the next index owns r.
// Pitfall: the tree is 1-based (index i lives at position i + 1); update adds the
// difference weight - weights[i], not weight, and keeps total in sync for pickIndex.
// T: build O(n log n), update / pickFor O(log n); S: O(n).
class DynamicWeightedPicker {
    // weights[i] = current w[i]
    vector<int> weights;
    // tree[x] = sum of w over 1-based positions (x - (x & -x), x]
    vector<long long> tree;
    // sum of the current weights
    long long total = 0;
    // highest power of two <= n: the first step of the descent
    int topStep = 1;

    // adds delta to the weight at 0-based index i
    void add(int i, long long delta) {
        int n = weights.size();
        for (int x = i + 1; x <= n; x += x & -x) { tree[x] += delta; }
        total += delta;
    }

public:
    DynamicWeightedPicker(const vector<int>& w) : weights(w), tree(w.size() + 1, 0) {
        for (int i = 0; i < (int)w.size(); i++) { add(i, w[i]); }
        while (topStep * 2 <= (int)w.size()) { topStep *= 2; }
    }

    void update(int i, int weight) {
        add(i, weight - weights[i]);
        weights[i] = weight;
    }

    // r in [0, total): the first index whose prefix sum is > r
    int pickFor(long long r) {
        int n = weights.size();
        // pos = largest 1-based position whose prefix sum is <= r
        int pos = 0;
        // r minus the prefix sum up to pos
        long long remaining = r;
        for (int step = topStep; step > 0; step /= 2) {
            if (pos + step <= n && tree[pos + step] <= remaining) {
                pos += step;
                remaining -= tree[pos];
            }
        }
        // 1-based position pos + 1 owns r: that is 0-based index pos
        return pos;
    }

    int pickIndex() { return pickFor(rand() % total); }
};

// follow-up: O(1) per pick, alias method (Vose). Give each index a column of height total
// and scale w[i] by n, so the scaled weights fill the n columns exactly. A light index
// (scaled < total) keeps the bottom of its column and its alias, a heavy index, fills
// the rest; the heavy index loses that much and may turn light. A pick is one random
// column plus one random coin in [0, total). Integers keep every share exact.
// Pitfall: after topping up a light column, re-file the heavy index as light or heavy;
// with exact integers every index left over holds exactly total.
// T: build O(n), pick O(1); S: O(n).
class AliasPicker {
    // keep[c] = coins in [0, keep[c]) return c itself
    vector<long long> keep;
    // alias[c] = index returned for coins >= keep[c]
    vector<int> alias;
    // sum of w = height of each column
    long long total = 0;

public:
    AliasPicker(const vector<int>& w) : keep(w.size(), 0), alias(w.size(), 0) {
        int n = w.size();
        for (int weight : w) { total += weight; }
        // scaled[i] = w[i] * n, the part of index i not placed yet
        vector<long long> scaled(n);
        // indices with scaled below / at least total
        vector<int> light;
        vector<int> heavy;
        for (int i = 0; i < n; i++) {
            scaled[i] = (long long)w[i] * n;
            if (scaled[i] < total) {
                light.push_back(i);
            } else {
                heavy.push_back(i);
            }
        }
        while (!light.empty() && !heavy.empty()) {
            int small = light.back();
            light.pop_back();
            int big = heavy.back();
            heavy.pop_back();
            keep[small] = scaled[small];
            alias[small] = big;
            scaled[big] -= total - scaled[small];
            if (scaled[big] < total) {
                light.push_back(big);
            } else {
                heavy.push_back(big);
            }
        }
        for (int i : heavy) {
            keep[i] = total;
            alias[i] = i;
        }
    }

    // column in [0, n), coin in [0, total)
    int pickFor(int column, long long coin) {
        return coin < keep[column] ? column : alias[column];
    }

    int pickIndex() { return pickFor(rand() % (int)keep.size(), rand() % total); }
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

// applies every [i, weight] update, then maps each draw to an index
void testDynamic(const string& label, const vector<int>& w,
                 const vector<vector<int>>& updates, const vector<int>& draws,
                 const vector<int>& want) {
    DynamicWeightedPicker picker(w);
    for (const vector<int>& update : updates) { picker.update(update[0], update[1]); }
    vector<int> got;
    for (int r : draws) { got.push_back(picker.pickFor(r)); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

// every (column, coin) pair is equally likely, so index i must win exactly w[i] * n of
// the n * total pairs
void testAlias(const string& label, const vector<int>& w) {
    AliasPicker picker(w);
    int n = w.size();
    long long total = 0;
    for (int weight : w) { total += weight; }
    // wins[i] = (column, coin) pairs that return i
    vector<long long> wins(n, 0);
    for (int column = 0; column < n; column++) {
        for (long long coin = 0; coin < total; coin++) {
            wins[picker.pickFor(column, coin)]++;
        }
    }
    bool ok = true;
    for (int i = 0; i < n; i++) { ok = ok && wins[i] == (long long)w[i] * n; }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-to-three", {1, 3}, {0, 1, 3}, {0, 1, 1});
    test("boundaries", {2, 2, 2}, {1, 2, 3, 4, 5}, {0, 1, 1, 2, 2});
    test("single", {5}, {0, 4}, {0, 0});
    test("heavy-middle", {1, 10, 1}, {0, 1, 10, 11}, {0, 1, 1, 2});
    testSample("sample-1-to-3", {1, 3}, {{0, 0.25}, {1, 0.75}});

    testDynamic("dyn-no-update", {1, 3}, {}, {0, 1, 3}, {0, 1, 1});
    testDynamic("dyn-grow-middle", {2, 2, 2}, {{1, 5}}, {1, 2, 6, 7, 8}, {0, 1, 1, 2, 2});
    testDynamic("dyn-zero-weight", {3, 1}, {{0, 0}}, {0}, {1});
    testDynamic("dyn-five", {1, 1, 1, 1, 1}, {{4, 3}}, {3, 4, 6}, {3, 4, 4});

    testAlias("alias-1-to-3", {1, 3});
    testAlias("alias-equal", {2, 2, 2});
    testAlias("alias-zero-weight", {5, 1, 0, 2});
    testAlias("alias-single", {7});
}

// follow-up: recorded draws sorted -> one merge pass with a pointer into prefixWeights,
//   O(n + D) instead of O(D log n):
//     int index = 0;
//     for (int r : draws) {
//         while (prefixWeights[index] <= r) { index++; }
//         picks.push_back(index);
//     }
// follow-up: weights updated between picks -> DynamicWeightedPicker above
// follow-up: O(1) per pick -> AliasPicker above
// follow-up: draw is a double u in [0, 1) -> scale it, same upper_bound (an int prefix
//   compares fine against a double):
//     double target = u * prefixWeights.back();
//     auto it = upper_bound(prefixWeights.begin(), prefixWeights.end(), target);
