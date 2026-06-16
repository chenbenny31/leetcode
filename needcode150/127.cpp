// bfs, T :O(nm^2), n = wordList size, m = word length, S: O(mn)

#include <string>
#include <unordered_set>
#include <vector>

class Solution {
public:
    int ladderLength(std::string beginWord, std::string endWord,
                     std::vector<std::string>& wordList) {
        std::unordered_set<std::string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) { return 0; }

        std::vector<std::string> q;
        q.reserve(wordList.size());
        int head = 0;
        q.push_back(beginWord);
        wordSet.erase(beginWord);
        int steps = 1;

        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;
            for (int i = 0; i < sz; i++) {
                std::string word = q[head];
                head++;

                for (int j = 0; j < static_cast<int>(word.size()); j++) {
                    char orig = word[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == orig) { continue; }
                        word[j] = c;
                        if (word == endWord) { return steps + 1; }
                        if (wordSet.count(word)) {
                            wordSet.erase(word);
                            q.push_back(word);
                        }
                    }
                    word[j] = orig;
                }
            }
            steps++;
        }
        return 0;
    }
};

// bi-directional bfs, T: O(nm^2), S: O(mn)

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm> // std::swap

class Solution {
public:
    int ladderLength(std::string beginWord, std::string endWord,
                     std::vector<std::string>& wordList) {
        std::unordered_set<std::string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) { return 0; }

        std::unordered_set<std::string> front{beginWord}, back{endWord};
        wordSet.erase(beginWord);
        wordSet.erase(endWord);
        int steps = 1;

        while (!front.empty() && !back.empty()) {
            if (front.size() > back.size()) { std::swap(front, back); }
            std::unordered_set<std::string> next;

            for (auto& word : front) {
                std::string w = word;

                for (int j = 0; j < static_cast<int>(w.size()); j++) {
                    char orig = w[j];
                    
                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == orig) { continue; }
                        w[j] = c;
                        if (back.count(w)) { return steps + 1; }
                        if (wordSet.count(w)) {
                            next.insert(w);
                            wordSet.erase(w);
                        }
                    }
                    w[j] = orig;
                }
            }
            front = std::move(next);
            steps++;
        }
        return 0;
    }
};

// erase from wordSet on visit: prevents revisiting, O(1) amortized erase
// bi-directional search space reduction: O(2*26^(d/2)), d=depth, exponentail reduction
// front = std::move(next): O(1) buffer transfer, no copy of next frontier
// if (front.size() > back.size()): always expand smaller frontier
