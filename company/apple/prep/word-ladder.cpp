// tranform beginWord into endWord, chaning one letter at a time
// every intermediate word must be in wordList
// return the num of words in the shortest path, including both end, or 0 if none

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

// BFS by layers, neighbors = all one-ltter edits in word dict
// T :O(NL*26), S: O(NL), N = num of words, L = word-len
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) { return 0; }

    queue<string> q; // words reached in cur layer
    q.push(beginWord);
    dict.erase(beginWord);
    int steps = 1;

    while (!q.empty()) {
        int sz = q.size(); // sz by layer
        for (int i = 0; i < sz; i++) {
            string cur = q.front();
            q.pop();
            if (cur == endWord) { return steps; }

            for (char& c : cur) {
                char orig = c;
                for (char ch = 'a'; ch <= 'z'; ch++) {
                    c = ch;
                    if (dict.count(cur)) { // compound by: if (dict.erase(cur))
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

// bi-directional BFS, T:O(NL*26), S: O(NL)
int ladderLengthBidir(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    unordered_set<string> dict(wordList.begin(), wordList.end());
    if (!dict.count(endWord)) { return 0; }

    // current frontier reached from each end
    unordered_set<string> front = {beginWord};
    unordered_set<string> back = {endWord};
    dict.erase(beginWord);
    dict.erase(endWord);
    int steps = 1;

    while (!front.empty() && !back.empty()) {
        if (front.size() > back.size()) { swap(front, back); }
        unordered_set<string> nxt; // next layer of cur expanding side
        for (string word : front) {
            for (char& c : word) {
                char orig = c;
                for (char ch = 'a'; ch <= 'z'; ch++) {
                    if (ch == orig) { continue; }
                    c = ch;
                    if (back.count(word)) { return steps + 1; }
                    if (dict.erase(word)) { nxt.insert(word); }
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
