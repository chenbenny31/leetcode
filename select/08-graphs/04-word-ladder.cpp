// LC 127 Word Ladder (Hard). Pattern: BFS shortest path on implicit graph.
// Transform beginWord into endWord one letter at a time; every intermediate word must be
// in wordList. Return the number of words in the shortest sequence, or 0 if impossible.

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

// BFS by layers; neighbors = one-letter edits found in the dictionary.
// T: O(N * L^2 * 26), S: O(N * L); N = words, L = word length (hashing costs L).
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

// follow-up: all shortest paths (LC 126) -> parents[nxt] collects every cur of the layer;
//   erase visited words per layer, not per push; DFS back from endWord
// follow-up: faster -> bidirectional BFS, always expand the smaller frontier set
