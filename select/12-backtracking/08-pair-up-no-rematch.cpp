// Pair Up Players Without Rematches (interview report) — backtracking perfect matching
// Players 0..n-1 (n even); played = pairs that already played each other. Return n / 2
// pairs covering every player with no pair in played, or {} when impossible.
// Clarify: n even and small (<= ~16); played unordered and may repeat; any valid answer;
//   order of the output pairs.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// met[a][b] = a and b already played; paired[p] = p is in pairs; pairs = chosen so far
bool dfs(const vector<vector<bool>>& met, vector<bool>& paired,
         vector<pair<int, int>>& pairs) {
    int n = met.size();
    // the lowest unpaired player must get a partner, so only its partner is a choice
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

// Core idea: take the lowest unpaired player, try every later unpaired partner that is
// not a rematch, recurse on the rest and undo when the rest cannot be paired.
// T: O((n - 1) * (n - 3) * ... * 1) worst case, S: O(n^2) for the matrix.
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

// follow-up (ideas only):
// - why not greedy / union-find -> a pair that looks fine now can strand the last two
//   players (they already met); this is matching, not connectivity, so choices need undo
// - count all valid pairings -> same dfs, add up the counts over the partners instead of
//   returning at the first success
// - max-weight pairing (or faster yes / no) for n <= ~20 -> bitmask DP over the set of
//   paired players: pair the lowest unset bit with each other unset bit, O(2^n * n)
// - large n -> maximum matching in a general graph (Edmonds' blossom); name it only
// - mahjong winning hand -> same shape: pick the pair, then peel a triple or a run that
//   starts at the smallest remaining tile, undo on failure
