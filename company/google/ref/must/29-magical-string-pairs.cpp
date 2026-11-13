// Magical String Pairs (Google, interview report)
// (1) count triples (i, j, k), i <= j < k: s[i..j] and s[j+1..k] have the same char set
// (2) the 2 parts must have the same letter counts (anagrams, so equal len)
//
// clarify:
// n -> assume 1e5
// answer size -> grows like n^3 / 6, long long
// part lens -> both non-empty; may differ in (1)
// "same set" -> ignores counts
// alphabet -> lowercase only, 26-bit mask

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::sort;
using std::min;

// (1) solution 1: fix the split + count masks; simple baseline
// core idea: fix split; walk start left counting masks of s[start..split]
// then walk end right, add how many left parts have the same mask
// T: O(n^2), S: O(n)
long long countMagical(const string& s) {
    int n = s.size();
    long long total = 0;
    for (int split = 0; split + 1 < n; split++) {
        // leftCount[mask] = num of starts whose s[start..split] has this letter set
        unordered_map<int, long long> leftCount;
        int mask = 0;
        for (int start = split; start >= 0; start--) {
            mask |= 1 << (s[start] - 'a');
            leftCount[mask]++;
        }
        mask = 0;
        for (int end = split + 1; end < n; end++) {
            mask |= 1 << (s[end] - 'a');
            auto it = leftCount.find(mask);
            if (it != leftCount.end()) { total += it->second; }
        }
    }
    return total;
}

// (1) solution 2: chains of nested sets; for n = 1e5
// core idea: per split each side has <= 26 nested sets, each held by a range of starts
// equal sets have equal sizes -> compare the 2 chains step by step, add starts * ends
// T: O(26 n log 26), S: O(26 n)
long long countMagicalFast(const string& s) {
    int n = s.size();
    // nextAt[p][c] = first idx >= p holding letter c, n if none
    vector<vector<int>> nextAt(n + 1, vector<int>(26, n));
    for (int p = n - 1; p >= 0; p--) {
        nextAt[p] = nextAt[p + 1];
        nextAt[p][s[p] - 'a'] = p;
    }
    // lastAt[c] = last idx <= split holding letter c, -1 if none
    vector<int> lastAt(26, -1);
    long long total = 0;
    for (int split = 0; split + 1 < n; split++) {
        lastAt[s[split] - 'a'] = split;
        // left / right = (position, letter), nearest to the split first
        vector<pair<int, int>> left;
        vector<pair<int, int>> right;
        for (int c = 0; c < 26; c++) {
            if (lastAt[c] >= 0) { left.push_back({lastAt[c], c}); }
            if (nextAt[split + 1][c] < n) { right.push_back({nextAt[split + 1][c], c}); }
        }
        sort(left.rbegin(), left.rend());
        sort(right.begin(), right.end());

        int leftMask = 0;
        int rightMask = 0;
        int steps = min(left.size(), right.size());
        for (int step = 0; step < steps; step++) {
            leftMask |= 1 << left[step].second;
            rightMask |= 1 << right[step].second;
            if (leftMask != rightMask) { continue; }
            // starts in (next left position, this one] keep this set; ends likewise
            int leftStop = step + 1 < (int)left.size() ? left[step + 1].first : -1;
            int rightStop = step + 1 < (int)right.size() ? right[step + 1].first : n;
            long long leftWays = left[step].first - leftStop;
            long long rightWays = rightStop - right[step].first;
            total += leftWays * rightWays;
        }
    }
    return total;
}

// add delta to diff[letter]; keep nonZero = num of letters w/ diff != 0
void addToDiff(vector<int>& diff, int& nonZero, int letter, int delta) {
    if (diff[letter] == 0) { nonZero++; }
    diff[letter] += delta;
    if (diff[letter] == 0) { nonZero--; }
}

// (2) anagram halves
// core idea: fix split, grow both parts by 1 letter per step; diff = left cnt - right cnt
// parts match when no letter has a non-zero diff
// T: O(n^2), S: O(1)
long long countAnagramHalves(const string& s) {
    int n = s.size();
    long long total = 0;
    for (int split = 0; split + 1 < n; split++) {
        // diff[c] = cnt of letter c in left part minus in right part
        vector<int> diff(26, 0);
        int nonZero = 0;
        for (int len = 1; split - len + 1 >= 0 && split + len < n; len++) {
            addToDiff(diff, nonZero, s[split - len + 1] - 'a', 1);
            addToDiff(diff, nonZero, s[split + len] - 'a', -1);
            if (nonZero == 0) { total++; }
        }
    }
    return total;
}

void test(const string& label, const string& s, long long want) {
    bool ok = countMagical(s) == want && countMagicalFast(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testAnagram(const string& label, const string& s, long long want) {
    cout << label << (countAnagramHalves(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-letter", "aaa", 4);
    test("abab", "abab", 1);
    test("no-match", "ab", 0);
    test("mirror", "abba", 2);
    testAnagram("anagram-same-letter", "aaa", 2);
    testAnagram("anagram-mirror", "abba", 2);
    testAnagram("anagram-abab", "abab", 1);
    testAnagram("anagram-none", "ab", 0);
}

// follow-up:
// (1) parts must also have equal len -> on a matching step, intersect the 2 len ranges
// (1) left lens -> split - left pos + 1 .. split - leftStop
// (1) right lens -> right pos - split .. rightStop - 1 - split
// (2) w/o the nonZero counter -> rescanning 26 letters per step is O(26 n^2)
