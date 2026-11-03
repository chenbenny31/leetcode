// LC 127: transform beginWord into endWord changing one letter at a time; every
// intermediate word must be in wordList. Return the number of words in the shortest
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
using std::swap;

// straightforward: BFS by layers; neighbors = all one-letter edits in the dictionary.
// T: O(N * L * 26), S: O(N * L); N = words, L = word length.
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());   // unvisited words
    if (!dict.count(endWord)) { return 0; }
    queue<string> q;   // words reached in the current layer
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

// improved: bidirectional BFS; grow the smaller frontier each round until it touches
// the other one. Same worst case, but each side only searches about half the depth,
// which shrinks the explored layers a lot when branching is high.
// T: O(N * L * 26), S: O(N * L).
int ladderLengthBidir(const string& beginWord, const string& endWord,
                      const vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());   // unvisited words
    if (!dict.count(endWord)) { return 0; }
    // current frontier reached from each end
    unordered_set<string> front = {beginWord}, back = {endWord};
    dict.erase(beginWord);
    dict.erase(endWord);
    int steps = 1;
    while (!front.empty() && !back.empty()) {
        if (front.size() > back.size()) { swap(front, back); }
        unordered_set<string> nxt;   // next layer of the side being expanded
        for (string w : front) {
            for (char& c : w) {
                char orig = c;
                for (char ch = 'a'; ch <= 'z'; ch++) {
                    if (ch == orig) { continue; }
                    c = ch;
                    // the two searches meet: one more edge joins them
                    if (back.count(w)) { return steps + 1; }
                    if (dict.count(w)) {
                        dict.erase(w);
                        nxt.insert(w);
                    }
                }
                c = orig;
            }
        }
        front = nxt;
        steps++;
    }
    return 0;
}

void test(const string& label, const string& beginWord, const string& endWord,
          const vector<string>& wordList, int want) {
    bool ok = ladderLength(beginWord, endWord, wordList) == want &&
              ladderLengthBidir(beginWord, endWord, wordList) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", "aa", "bb", {"ab", "bb"}, 3);
    test("one-step", "aa", "ab", {"ab"}, 2);
    test("no-end", "aa", "bb", {"ab"}, 0);
    test("no-path", "aa", "bb", {"cc", "bb"}, 0);
}
