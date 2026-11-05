// ByteDance / LC 127: transform beginWord into endWord changing one letter at a time;
// every intermediate word must be in wordList. Return the number of words in the shortest
// sequence (including both ends), or 0 if impossible.
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

// BFS by layers; neighbors = all one-letter edits found in the dictionary.
// T: O(N * L * 26), S: O(N * L); N = words, L = word length.
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    // unvisited dictionary words
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) { return 0; }
    // words reached in the current layer
    queue<string> q;
    q.push(beginWord);
    dict.erase(beginWord);
    int steps = 1;
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            string cur = q.front();
            q.pop();
            if (cur == endWord) { return steps; }
            for (char& c : cur) {
                char orig = c;
                for (char ch = 'a'; ch <= 'z'; ch++) {
                    c = ch;
                    // erase on push so a word enters the queue only once
                    if (dict.count(cur)) {
                        dict.erase(cur);
                        q.push(cur);
                    }
                }
                c = orig;
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
    test("lc-ex1", "hit", "cog", {"hot", "dot", "dog", "lot", "log", "cog"}, 5);
    test("end-missing", "hit", "cog", {"hot", "dot", "dog", "lot", "log"}, 0);
    test("unreachable", "hit", "cog", {"hot", "cog"}, 0);
}

// follow-up: return the path -> parent[nxt] = cur (keep an unedited copy of cur) on push,
//   walk back from endWord, reverse
// follow-up: all shortest paths (LC 126) -> parents[nxt] collects every cur of the layer;
//   erase visited words per layer, not per push; DFS back from endWord
// follow-up: faster -> bidirectional BFS, expand the smaller frontier set, stop when a
//   neighbor is in the other set; ~2 * b^(d/2) states instead of b^d
