// LC 127 Word Ladder: change one letter at a time, every intermediate word must be in
// wordList. Return the number of words in the shortest sequence from beginWord to
// endWord (both counted), or 0 if there is none.
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

// BFS by layers; an edge joins words that differ in one letter.
// T: O(N * L * 26), S: O(N * L); N = words, L = word length.
int ladderLength(const string& beginWord, const string& endWord,
                 const vector<string>& wordList) {
    unordered_set<string> unseen(wordList.begin(), wordList.end());   // not queued yet
    unseen.erase(beginWord);
    queue<string> q;   // words of the current layer, then of the next
    q.push(beginWord);
    int steps = 1;
    while (!q.empty()) {
        // freeze the layer size: words pushed now belong to the next layer
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            string cur = q.front();
            q.pop();
            if (cur == endWord) { return steps; }
            string nxt = cur;
            for (char& c : nxt) {
                char orig = c;
                for (char ch = 'a'; ch <= 'z'; ch++) {
                    c = ch;
                    if (unseen.count(nxt) == 0) { continue; }
                    // erase on push so a word enters the queue only once
                    unseen.erase(nxt);
                    q.push(nxt);
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
    cout << label << (ladderLength(beginWord, endWord, wordList) == want ? " PASS\n"
                                                                         : " FAIL\n");
}

int main() {
    test("chain", "aa", "cc", {"ab", "cb", "cc"}, 4);
    test("shortcut", "aa", "cc", {"ab", "cb", "ac", "cc"}, 3);
    test("end-missing", "aa", "cc", {"ab", "cb"}, 0);
    test("no-path", "aa", "cc", {"zz", "cc"}, 0);
}
