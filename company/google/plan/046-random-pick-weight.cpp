// LC 528 Random Pick with Weight (Google-tagged)
// (1) weights w > 0; pickIndex() returns i w/ probability w[i] / total
// (1) pickFor(r) maps draw r in [0, total) to its idx; pickAll maps recorded draws
// (2) update(i, weight)
// (3) O(1) per pick
// reported: random pick w/ weight using recorded draws
//
// clarify:
// weight 0 -> never picked
// total -> fits in int
// draw range -> [0, total) here; [1, total] is a follow-up
// picks vs updates -> prefix sums if static, Fenwick if updates, alias if O(1) pick

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

// (1) prefix sums + binary search
// core idea: idx i owns [prefix[i - 1], prefix[i]); draw r -> first prefix > r
// T: build O(n), pick O(log n), pickAll O(D log n), S: O(n)
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

    // brute force: add weights until running sum > r, O(n)
    int pickForLinear(int r) {
        int running = 0;
        int n = weights.size();
        for (int i = 0; i < n; i++) {
            running += weights[i];
            if (running > r) { return i; }
        }
        return n - 1;
    }

    // binary search the prefix sums, O(log n)
    int pickFor(int r) {
        // first i w/ prefix > r (prefix == r is where i + 1 starts)
        auto it = upper_bound(prefixWeights.begin(), prefixWeights.end(), r);
        return it - prefixWeights.begin();
    }

    int pickIndex() { return pickFor(rand() % prefixWeights.back()); }

    // recorded draws: same lookup per draw
    vector<int> pickAll(const vector<int>& draws) {
        vector<int> picks;
        for (int r : draws) { picks.push_back(pickFor(r)); }
        return picks;
    }
};

// (2) Fenwick tree: update(i, weight)
// core idea: Fenwick keeps prefix sums under updates; pickFor descends the tree
// descent finds last 1-based pos w/ prefix <= r; the next idx owns r
// T: build O(n log n), update / pick O(log n), S: O(n)
class DynamicWeightedPicker {
    // weights[i] = current w[i]
    vector<int> weights;
    // tree[x] = sum of w over 1-based positions (x - (x & -x), x]
    vector<long long> tree;
    // sum of the current weights
    long long total = 0;
    // highest power of two <= n: the first step of the descent
    int topStep = 1;

    // add delta to weight at 0-based idx i
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

    // r in [0, total): first idx whose prefix sum > r
    int pickFor(long long r) {
        int n = weights.size();
        // pos = largest 1-based position w/ prefix sum <= r
        int pos = 0;
        // r minus the prefix sum up to pos
        long long remaining = r;
        for (int step = topStep; step > 0; step /= 2) {
            if (pos + step <= n && tree[pos + step] <= remaining) {
                pos += step;
                remaining -= tree[pos];
            }
        }
        // 1-based pos + 1 owns r = 0-based idx pos
        return pos;
    }

    int pickIndex() { return pickFor(rand() % total); }
};

// (3) alias method (Vose): O(1) pick
// core idea: scale w[i] by n -> n columns of height total, filled exactly
// light idx keeps its column bottom; a heavy idx (alias) fills the rest, then is re-filed
// T: build O(n), pick O(1), S: O(n)
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

    // one pick = random column in [0, n) + coin in [0, total)
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

// wantShare[i] = expected fraction of pickIndex() calls returning i
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

// apply every [i, weight] update, then map each draw to an idx
void testDynamic(const string& label, const vector<int>& w,
                 const vector<vector<int>>& updates, const vector<int>& draws,
                 const vector<int>& want) {
    DynamicWeightedPicker picker(w);
    for (const vector<int>& update : updates) { picker.update(update[0], update[1]); }
    vector<int> got;
    for (int r : draws) { got.push_back(picker.pickFor(r)); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

// idx i must win exactly w[i] * n of the n * total (column, coin) pairs
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

// follow-up:
// sorted recorded draws -> one merge pass w/ pointer into prefixWeights, O(n + D)
// draw = double u in [0, 1) -> scale by total, same upper bound
// draw in [1, total] -> lower bound instead of upper bound
// rand() % total slightly biased -> mt19937 + uniform_int_distribution<int>
// total above int -> long long prefix sums
