// LC 127 Word Ladder (Hard) — BFS shortest path on an implicit graph
// Transform beginWord into endWord one letter at a time; every intermediate word must be
// in wordList. Return the number of words in the shortest sequence, or 0 if impossible.
// Clarify: endWord must be in wordList, beginWord need not be; same length, lowercase
//   only; the answer counts words, not changes; beginWord == endWord.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_set;

// Core idea: BFS by layers from beginWord; the neighbors of a word are its one-letter
// edits that are still in the dictionary set, and the first layer that holds endWord is
// the answer. T: O(N * 26 * L^2), S: O(N * L) for N words of length L (a lookup is O(L)).
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    // dictionary words not visited yet
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) { return 0; }
    // words reached in the current layer
    queue<string> q;
    q.push(beginWord);
    dict.erase(beginWord);
    int steps = 1;
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            string cur = q.front();
            q.pop();
            if (cur == endWord) { return steps; }
            for (char& slot : cur) {
                char original = slot;
                for (char letter = 'a'; letter <= 'z'; letter++) {
                    slot = letter;
                    // erase on push so a word enters the queue only once
                    if (dict.count(cur)) {
                        dict.erase(cur);
                        q.push(cur);
                    }
                }
                slot = original;
            }
        }
        steps++;
    }
    return 0;
}

void test(const string& label, const string& beginWord, const string& endWord,
          const vector<string>& wordList, int want) {
    int got = ladderLength(beginWord, endWord, wordList);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cat-to-dog", "cat", "dog", {"cot", "cog", "dog"}, 4);
    test("one-step", "cat", "cot", {"cot"}, 2);
    test("end-missing", "cat", "dog", {"cot", "cog"}, 0);
    test("unreachable", "cat", "dog", {"cot", "dog"}, 0);
}

// follow-up (ideas only):
// - faster in practice -> bidirectional BFS: a frontier set from each end, always expand
//   the smaller one, stop when a neighbor is in the other frontier
// - return all shortest ladders (LC 126) -> BFS by levels where parents[nxt] collects
//   every cur of the layer (erase visited words per layer, not per push), then backtrack
//   from endWord over the parent lists
// - open the lock (LC 752) / minimum genetic mutation (LC 433) -> same BFS on implicit
//   states: neighbors are the 8 wheel turns / the one-letter edits over ACGT, and the
//   dead ends / the bank play the role of the set
// - sliding puzzle (LC 773) -> state = the board as a string; neighbors swap the 0 with
//   an adjacent cell; visited set of strings
// - why BFS and not DFS -> edges are unweighted, so BFS reaches every word first along a
//   shortest path; DFS would have to try all paths
