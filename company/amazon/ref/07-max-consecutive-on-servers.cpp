// serverStates is a 0/1 string (1 = ON). One operation flips every state in a contiguous
// range; you may do at most k operations. Return the max number of consecutive ON
// servers you can get.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::max;

// A stretch can be made all ON iff it holds at most k blocks of 0s (flip each block).
// straightforward: from every start, extend while the 0-block count stays <= k.
// T: O(n^2), S: O(1).
int maxOn(const string& s, int k) {
    int n = s.size(), best = 0;
    for (int i = 0; i < n; i++) {
        int blocks = 0;
        for (int j = i; j < n; j++) {
            if (s[j] == '0' && (j == i || s[j - 1] == '1')) { blocks++; }
            if (blocks > k) { break; }
            best = max(best, j - i + 1);
        }
    }
    return best;
}

// improved: compress into runs, then slide a window over runs holding <= k zero runs.
// T: O(n), S: O(n).
int maxOnRuns(const string& s, int k) {
    // runs[i] = (char, length) of the i-th maximal run
    vector<pair<char, int>> runs;
    for (char c : s) {
        if (runs.empty() || runs.back().first != c) {
            runs.push_back({c, 1});
        } else {
            runs.back().second++;
        }
    }
    int best = 0, len = 0, zeros = 0, left = 0;
    int r = runs.size();
    for (int right = 0; right < r; right++) {
        len += runs[right].second;
        if (runs[right].first == '0') { zeros++; }
        while (zeros > k) {
            len -= runs[left].second;
            if (runs[left].first == '0') { zeros--; }
            left++;
        }
        best = max(best, len);
    }
    return best;
}

void test(const string& label, const string& s, int k, int want) {
    bool ok = maxOn(s, k) == want && maxOnRuns(s, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k-zero", "1101110", 0, 3);
    test("bridge", "1101011", 1, 4);
    test("all-blocks", "1101011", 2, 7);
    test("all-off", "000", 1, 3);
    test("off-k-zero", "000", 0, 0);
    test("big-k", "0101", 5, 4);
}
