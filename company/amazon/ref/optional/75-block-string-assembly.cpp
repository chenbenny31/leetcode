// Each block is a set of uppercase letters. Can target be spelled by taking one letter
// from a distinct block for each position (each block used at most once, any order)?

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// try every unused block holding target[pos] for position pos, undo on failure
bool dfs(const vector<string>& blocks, const string& target, int pos,
         vector<bool>& used) {
    if (pos == (int)target.size()) { return true; }
    int b = blocks.size();
    for (int block = 0; block < b; block++) {
        if (used[block] || blocks[block].find(target[pos]) == string::npos) { continue; }
        used[block] = true;
        if (dfs(blocks, target, pos + 1, used)) { return true; }
        used[block] = false;
    }
    return false;
}

// straightforward: backtracking over positions. T: O(b^t) worst case, S: O(b + t).
bool canForm(const vector<string>& blocks, const string& target) {
    vector<bool> used(blocks.size(), false);
    return dfs(blocks, target, 0, used);
}

// Kuhn's augmenting path: give position pos a block, moving earlier positions if needed
bool augment(const vector<string>& blocks, const string& target, int pos,
             vector<bool>& seen, vector<int>& owner) {
    int b = blocks.size();
    for (int block = 0; block < b; block++) {
        if (seen[block] || blocks[block].find(target[pos]) == string::npos) { continue; }
        seen[block] = true;
        if (owner[block] == -1 || augment(blocks, target, owner[block], seen, owner)) {
            owner[block] = pos;
            return true;
        }
    }
    return false;
}

// improved: positions vs blocks is bipartite matching; succeed iff every position gets
// a block. T: O(t * b * 26), S: O(b).
bool canFormMatching(const vector<string>& blocks, const string& target) {
    // owner[block] = position that block spells, -1 if free
    vector<int> owner(blocks.size(), -1);
    int t = target.size();
    for (int pos = 0; pos < t; pos++) {
        vector<bool> seen(blocks.size(), false);
        if (!augment(blocks, target, pos, seen, owner)) { return false; }
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
