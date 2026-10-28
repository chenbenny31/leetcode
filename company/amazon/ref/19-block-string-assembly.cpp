// Each block is a set of uppercase letters. Can target be spelled by taking one letter
// from a distinct block for each position (each block used at most once, any order)?

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// try every unused block holding target[i] for position i, undo on failure
bool dfs(const vector<string>& blocks, const string& target, int i, vector<bool>& used) {
    if (i == (int)target.size()) { return true; }
    int b = blocks.size();
    for (int j = 0; j < b; j++) {
        if (used[j] || blocks[j].find(target[i]) == string::npos) { continue; }
        used[j] = true;
        if (dfs(blocks, target, i + 1, used)) { return true; }
        used[j] = false;
    }
    return false;
}

// straightforward: backtracking over positions. T: O(b^t) worst case, S: O(b + t).
bool canForm(const vector<string>& blocks, const string& target) {
    vector<bool> used(blocks.size(), false);
    return dfs(blocks, target, 0, used);
}

// Kuhn's augmenting path: give position i a block, moving earlier positions if needed
bool augment(const vector<string>& blocks, const string& target, int i,
             vector<bool>& seen, vector<int>& owner) {
    int b = blocks.size();
    for (int j = 0; j < b; j++) {
        if (seen[j] || blocks[j].find(target[i]) == string::npos) { continue; }
        seen[j] = true;
        if (owner[j] == -1 || augment(blocks, target, owner[j], seen, owner)) {
            owner[j] = i;
            return true;
        }
    }
    return false;
}

// improved: positions vs blocks is bipartite matching; succeed iff every position gets
// a block. T: O(t * b * 26), S: O(b).
bool canFormMatching(const vector<string>& blocks, const string& target) {
    // owner[j] = position that block j spells, -1 if free
    vector<int> owner(blocks.size(), -1);
    int t = target.size();
    for (int i = 0; i < t; i++) {
        vector<bool> seen(blocks.size(), false);
        if (!augment(blocks, target, i, seen, owner)) { return false; }
    }
    return true;
}

void test(const string& label, const vector<string>& blocks, const string& target,
          bool want) {
    bool ok = canForm(blocks, target) == want && canFormMatching(blocks, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("any-order", {"XB", "AY"}, "AB", true);
    test("reassign", {"AB", "A"}, "AB", true);
    test("one-block", {"AB", "CD"}, "AB", false);
    test("too-few", {"A"}, "AA", false);
    test("missing", {"ABC"}, "D", false);
}
