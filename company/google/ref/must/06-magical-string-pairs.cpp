// s is lowercase letters. Count triples (i, j, k) with i <= j < k where the substrings
// s[i..j] and s[j+1..k] contain exactly the same set of distinct characters.
//
// Clarify:
// - n up to? -> assume 1e5: the O(n^2) mask scan is the first answer, the 26-step
//   chains give O(26 n log 26)
// - the answer grows like n^3 / 6 -> long long
// - both parts non-empty (i <= j < k) -> yes; their lengths may differ
// - same set = same distinct letters, counts ignored -> "aab" and "ba" match
// - lowercase only -> a 26-bit int mask is the set
// Idea: fix the split j. Moving the start left, the left set only grows, and only at the
//   nearest earlier copy of a letter it lacks: at most 26 different sets, each held by a
//   range of starts. Same for the ends on the right. Equal sets have equal sizes, so step
//   s on the left can only match step s on the right; add starts * ends when they match.
// Pitfalls:
// - the left part is s[start..split]: build its mask walking start leftward from split
// - long long for the total and for leftWays * rightWays
// - set lastAt[s[split]] = split before building the left chain (split is on the left)
// - left chain nearest first = positions descending; right chain ascending
// - a range ends at the next chain position, or at -1 / n when the chain runs out
// Tests to write: one letter repeated; "abab"; no match; length 1; mirror "abba".

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

// straightforward: fix the split j. Walk start leftward from split, counting the
// character-set mask of s[start..split]; then walk end rightward from split + 1 and add
// how many left parts have the same mask. T: O(n^2), S: O(n).
long long countMagical(const string& s) {
    int n = s.size();
    long long total = 0;
    for (int split = 0; split + 1 < n; split++) {
        // leftCount[mask] = number of starts whose s[start..split] has this letter set
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

// improved: as start moves left from split, the letter set of s[start..split] only grows
// when start reaches the nearest earlier copy of a new letter, so it takes at most 26
// nested values. Same on the right as end moves right. Two nested chains can share a set
// only at the same step (same size), so compare them step by step.
// T: O(26 n log 26), S: O(26 n).
long long countMagicalFast(const string& s) {
    int n = s.size();
    // nextAt[p][c] = first index >= p holding letter c, n if none
    vector<vector<int>> nextAt(n + 1, vector<int>(26, n));
    for (int p = n - 1; p >= 0; p--) {
        nextAt[p] = nextAt[p + 1];
        nextAt[p][s[p] - 'a'] = p;
    }
    // lastAt[c] = last index <= split holding letter c, -1 if none
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

// adds delta to diff[letter] and keeps nonZero = number of letters whose diff is not 0
void addToDiff(vector<int>& diff, int& nonZero, int letter, int delta) {
    if (diff[letter] == 0) { nonZero++; }
    diff[letter] += delta;
    if (diff[letter] == 0) { nonZero--; }
}

// follow-up: the two parts must have the same letter COUNTS (anagrams, so equal length).
// Fix the split and grow both parts by one letter per step; diff = left counts minus
// right counts, and the parts match when no letter has a non-zero diff.
// Clarify: counts include repeats ("ab" | "ba" matches, "aab" | "ab" does not).
// Pitfall: update nonZero on every change, not by rescanning 26 letters (O(26 n^2)).
// Tests to write: one letter repeated; mirror; "abab" (only the length-2 halves).
// T: O(n^2), S: O(1).
long long countAnagramHalves(const string& s) {
    int n = s.size();
    long long total = 0;
    for (int split = 0; split + 1 < n; split++) {
        // diff[c] = count of letter c in the left part minus in the right part
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
    test("single", "a", 0);
    test("mirror", "abba", 2);
    testAnagram("anagram-same-letter", "aaa", 2);
    testAnagram("anagram-mirror", "abba", 2);
    testAnagram("anagram-abab", "abab", 1);
    testAnagram("anagram-none", "ab", 0);
}

// follow-up: the two parts must also have equal length -> in countMagicalFast, count only
//   the lengths both ranges allow (length = split - start + 1 = end - split):
//     int lowLen = max(split - left[step].first + 1, right[step].first - split);
//     int highLen = min(split - leftStop, rightStop - 1 - split);
//     if (lowLen <= highLen) { total += highLen - lowLen + 1; }
// follow-up: equal letter counts (anagram halves) -> countAnagramHalves above
