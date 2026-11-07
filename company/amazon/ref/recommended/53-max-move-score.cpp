// Max move score: split s into pieces in order. A piece equal to moves[i] earns
// score[i]; otherwise the piece must be one character and earns 1 (a whiff). Return
// the max total score (0 for empty s). Moves are distinct, each at most 20 chars.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::unordered_map;

// Word-break style DP: from each reachable prefix, try a whiff or any move starting
// there. T: O(n * L^2) with L = longest move (substring hashing), S: O(n + total moves).
long long maxScore(const string& s, const vector<string>& moves,
                   const vector<int>& score) {
    // points[move] = score for that move
    unordered_map<string, int> points;
    int maxLen = 0;
    for (int i = 0; i < (int)moves.size(); i++) {
        points[moves[i]] = score[i];
        maxLen = max(maxLen, (int)moves[i].size());
    }
    int n = s.size();
    // best[i] = max score for the first i characters
    vector<long long> best(n + 1, 0);
    for (int i = 0; i < n; i++) {
        best[i + 1] = max(best[i + 1], best[i] + 1);
        for (int len = 1; len <= maxLen && i + len <= n; len++) {
            auto it = points.find(s.substr(i, len));
            if (it == points.end()) { continue; }
            best[i + len] = max(best[i + len], best[i] + it->second);
        }
    }
    return best[n];
}

void test(const string& label, const string& s, const vector<string>& moves,
          const vector<int>& score, long long want) {
    cout << label << (maxScore(s, moves, score) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("move-beats-whiffs", "AB", {"AB"}, {5}, 5);
    test("whiffs-only", "AB", {"C"}, {9}, 2);
    test("better-split", "ABC", {"AB", "BC", "C"}, {4, 6, 1}, 7);
    test("empty", "", {"A"}, {7}, 0);
}
