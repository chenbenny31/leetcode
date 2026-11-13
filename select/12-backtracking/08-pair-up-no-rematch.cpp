// Pair Up Players Without Rematches (interview report) — backtracking perfect matching
// players 0..n-1 (n even); played = pairs that already met
// return n / 2 pairs covering all players w/ no rematch, or {} if impossible
//
// clarify:
// n even and small (<= ~16) -> backtracking ok; odd n -> {}
// played unordered, may repeat -> symmetric bool matrix
// any valid answer -> stop at first success
// output pair order -> any

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// paired[p] = p is in pairs; pairs = chosen so far; true if the rest pairs up
bool dfs(const vector<vector<bool>>& met, vector<bool>& paired,
         vector<pair<int, int>>& pairs) {
    int n = met.size();
    // lowest unpaired player must get a partner, so only its partner is a choice
    int first = 0;
    while (first < n && paired[first]) { first++; }
    if (first == n) { return true; }
    paired[first] = true;
    for (int partner = first + 1; partner < n; partner++) {
        if (paired[partner] || met[first][partner]) { continue; }
        paired[partner] = true;
        pairs.push_back({first, partner});
        if (dfs(met, paired, pairs)) { return true; }
        pairs.pop_back();
        paired[partner] = false;
    }
    paired[first] = false;
    return false;
}

// core idea: lowest unpaired player tries each later non-rematch partner; undo on fail
// T: O((n - 1)!!), S: O(n^2) — (n - 1) * (n - 3) * ... * 1 pairings; S = met matrix
vector<pair<int, int>> pairUp(int n, const vector<pair<int, int>>& played) {
    vector<pair<int, int>> pairs;
    if (n % 2 != 0) { return pairs; }
    // met[a][b] = a and b already played each other
    vector<vector<bool>> met(n, vector<bool>(n, false));
    for (const auto& [a, b] : played) {
        met[a][b] = true;
        met[b][a] = true;
    }
    vector<bool> paired(n, false);
    dfs(met, paired, pairs);
    return pairs;
}

bool isValidPairing(int n, const vector<pair<int, int>>& played,
                    const vector<pair<int, int>>& pairs) {
    if ((int)pairs.size() != n / 2) { return false; }
    vector<bool> seen(n, false);
    for (const auto& [a, b] : pairs) {
        if (a == b || seen[a] || seen[b]) { return false; }
        seen[a] = true;
        seen[b] = true;
        for (const auto& [x, y] : played) {
            if ((a == x && b == y) || (a == y && b == x)) { return false; }
        }
    }
    return true;
}

void test(const string& label, int n, const vector<pair<int, int>>& played,
          bool possible) {
    vector<pair<int, int>> pairs = pairUp(n, played);
    bool ok = pairs.empty();
    if (possible) { ok = isValidPairing(n, played, pairs); }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("no-history", 4, {}, true);
    test("first-choice-strands-last-two", 4, {{2, 3}}, true);
    test("only-pair-is-rematch", 2, {{0, 1}}, false);
    test("played-everyone", 4, {{0, 1}, {0, 2}, {3, 0}}, false);
    return 0;
}

// follow-up:
// why not greedy / union-find -> a pair can strand the last two; matching needs undo
// cnt all valid pairings -> same dfs, sum cnts over partners instead of early return
// max-weight pairing, n <= ~20 -> bitmask DP: pair lowest unset bit, O(2^n * n)
// large n -> max matching in a general graph (Edmonds' blossom); name it only
// mahjong winning hand -> same shape: pick the pair, peel triple / run from smallest tile
