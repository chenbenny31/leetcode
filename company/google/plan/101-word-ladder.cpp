// LC 127 Word Ladder (Hard) — BFS shortest path on an implicit graph
// change beginWord -> endWord one letter at a time; each step must be in wordList
// return num of words in the shortest sequence, or 0 if impossible
//
// clarify:
// endWord not in wordList -> 0; beginWord need not be in it
// words -> same len, lowercase only
// answer -> counts words, not changes
// beginWord == endWord -> assume not

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

// core idea: BFS by layers; neighbors = one-letter edits still in dict
// T: O(N * 26 * L^2), S: O(N * L) — N words of len L, each lookup O(L)
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    // dict words not visited yet
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) { return 0; }
    // words reached in cur layer
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

// follow-up:
// faster in practice -> bidirectional BFS: expand smaller frontier, stop when they touch
// all shortest ladders (LC 126) -> parents[nxt] per layer (erase per layer), backtrack
// open the lock (LC 752) / genetic mutation (LC 433) -> same BFS on implicit states
// sliding puzzle (LC 773) -> state = board as string; swap 0 w/ adjacent; visited set
// why BFS not DFS -> unweighted edges: BFS reaches each word first by a shortest path
