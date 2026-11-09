// s is lowercase letters. Count triples (i, j, k) with i <= j < k where the substrings
// s[i..j] and s[j+1..k] contain exactly the same set of distinct characters.

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

void test(const string& label, const string& s, long long want) {
    bool ok = countMagical(s) == want && countMagicalFast(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-letter", "aaa", 4);
    test("abab", "abab", 1);
    test("no-match", "ab", 0);
    test("single", "a", 0);
    test("mirror", "abba", 2);
}

// follow-up: substrings must also have equal length / equal counts -> different problem
//   (hash of counts), mention only
