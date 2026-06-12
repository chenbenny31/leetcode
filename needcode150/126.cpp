// bfs + backtracking, T: O(nm^2 + k), n = wordList size, m = word length, k = total paths, S: O(nm)

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue> 

class Solution {
public:
    std::vector<std::vector<std::string>> findLadders(std::string beginWord, std::string endWord,
                                                      std::vector<std::string>& wordList) {
        std::unordered_set<std::string> wordSet(wordList.begin(), wordList.end());
        if (!wordSet.count(endWord)) { return {}; }

        // bfs, build level map, word -> shortest distance from beginWord
        std::unordered_map<std::string, int> dist;
        dist[beginWord] = 0;
        std::queue<std::string> q;
        q.push(beginWord);
        bool found = false;

        while (!q.empty() && !found) {
            int sz = static_cast<int>(q.size());
            std::unordered_set<std::string> level_visited;

            for (int i = 0; i < sz; i++) {
                std::string word = q.front(); q.pop();
                std::string next = word;

                for (int j = 0; j < static_cast<int>(word.size()); j++) {
                    char orig = next[j];

                    for (char c = 'a'; c <= 'z'; c++) {
                        if (c == orig) { continue; }
                        next[j] = c;
                        if (next == endWord) { found = true; }
                        if (wordSet.count(next) && !dist.count(next)) {
                            dist[next] = dist[word] + 1;
                            level_visited.insert(next);
                            q.push(next);
                        }
                    }
                    next[j] = orig;
                }
            }
            for (auto& w : level_visited) { wordSet.erase(w); }
        }

        // backtrack from endWord to beginWord
        std::vector<std::vector<std::string>> out;
        std::vector<std::string> path = {endWord};
        backtrack(endWord, beginWord, dist, out, path);
        return out;
    }

private:
    void backtrack(const std::string& word, const std::string& beginWord,
                   std::unordered_map<std::string, int>& dist,
                   std::vector<std::vector<std::string>>& out,
                   std::vector<std::string>& path) {
        if (word == beginWord) {
            out.push_back(std::vector<std::string>(path.rbegin(), path.rend()));
            return;
        }

        std::string prev = word;
        for (int i = 0; i < static_cast<int>(word.size()); i++) {
            char orig = prev[i];

            for (char c = 'a'; c <= 'z'; c++) {
                if (c == orig) { continue; }
                prev[i] = c;
                if (dist.count(prev) && dist[prev] == dist[word] - 1) {
                    path.push_back(prev);
                    backtrack(prev, beginWord, dist, out, path);
                    path.pop_back();
                }
            }
            prev[i] = orig;
        }
    }
};

// bfs builds shortest path DAG: dist[word] = shortest distance from beginWord, backtracking follows dist[prev] = dist[word] - 1
// level_visited: erase after level, words found in current level removed from wordSet, prevent mutual referencing
// backtrack with path.rbegin(), path.rend(): path built end-to-start during backtrack, reverse on collection
// why bfs then backtrack to collect paths: collecting paths during bfs need storing all partial paths
