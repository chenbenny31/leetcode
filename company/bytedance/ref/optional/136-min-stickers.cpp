// ByteDance / LC 691: min number of stickers (each reusable, letters cut out and
// rearranged, each letter covers one target letter) to spell target; -1 if impossible.
// Lowercase; <= 50 stickers of <= 20 letters, target <= 15.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// Fewest stickers to cover rest (kept sorted, so equal multisets share a memo key).
int dfs(const string& rest, const vector<vector<int>>& counts,
        unordered_map<string, int>& memo) {
    if (rest.empty()) {
        return 0;
    }
    if (memo.count(rest)) {
        return memo[rest];
    }

    int best = -1;
    for (const vector<int>& sticker : counts) {
        // only stickers holding rest[0]: some sticker must cover it, order is irrelevant
        if (sticker[rest[0] - 'a'] == 0) {
            continue;
        }

        // need[c] = copies of letter c still uncovered after using this sticker
        vector<int> need(26, 0);
        for (char ch : rest) {
            need[ch - 'a']++;
        }
        string next;
        for (int c = 0; c < 26; c++) {
            int left = need[c] - sticker[c];
            if (left > 0) {
                next += string(left, 'a' + c);
            }
        }

        int sub = dfs(next, counts, memo);
        if (sub != -1 && (best == -1 || sub + 1 < best)) {
            best = sub + 1;
        }
    }
    memo[rest] = best;
    return best;
}

// Memoized DFS over the multiset of uncovered letters.
// T: O(2^t * s * 26) for target length t and s stickers, S: O(2^t) memo entries.
int minStickers(const vector<string>& stickers, const string& target) {
    // counts[i][c] = copies of letter c on sticker i
    vector<vector<int>> counts;
    for (const string& s : stickers) {
        vector<int> count(26, 0);
        for (char ch : s) {
            count[ch - 'a']++;
        }
        counts.push_back(count);
    }

    // sort the target once; dfs keeps every rest string sorted
    vector<int> need(26, 0);
    for (char ch : target) {
        need[ch - 'a']++;
    }
    string rest;
    for (int c = 0; c < 26; c++) {
        rest += string(need[c], 'a' + c);
    }

    // memo[rest] = answer for that sorted uncovered multiset
    unordered_map<string, int> memo;
    return dfs(rest, counts, memo);
}

void test(const string& label, const vector<string>& stickers, const string& target,
          int want) {
    cout << label << (minStickers(stickers, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-stickers", {"ab", "bc"}, "abc", 2);
    test("reuse-one", {"aaa"}, "aaaa", 2);
    test("empty-target", {"xyz"}, "", 0);
    test("impossible", {"ab"}, "c", -1);
    test("double-full", {"abc", "a"}, "aabbcc", 2);
}

// follow-up: bitmask DP -> dp[mask] over 2^t target positions; sticker greedily fills
//   uncovered positions -> newMask, dp[newMask] = min(dp[mask] + 1), O(2^t * s * t)
// follow-up: prune stickers -> ignore letters not in target; drop a sticker dominated
//   (count <= on every letter) by another
// follow-up: return which stickers -> keep choice[rest] = best sticker idx, replay from
//   the sorted target
