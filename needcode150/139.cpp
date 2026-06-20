// word break, T: O(m*n^2), n = string length, m = avg word length, S: O(n)

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint> // uint8_t
#include <string_view> // C++17

class Solution {
public:
    bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
        std::unordered_set<std::string_view> words(wordDict.begin(), wordDict.end());
        std::string_view sv(s);
        int n = static_cast<int>(s.size());
        std::vector<uint8_t> dp(n + 1, 0); // dp[i] = 1 if s[0..i] can be break into wordDict
        dp[0] = 1; // empty prefix always valid

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] && words.count(sv.substr(j, i - j))) {
                    dp[i] = 1;
                    break;
                }
            }
        }
        return dp[n];
    }
};

// bfs, T: O(m*n^2), S: O(n)

#include <string>
#include <vector>
#include <unordered_set>
#include <cstdint> // uint8_t

class Solution {
public:
    bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
        std::unordered_set<std::string> words(wordDict.begin(), wordDict.end());
        int n = static_cast<int>(s.size());

        std::vector<uint8_t> visited(n + 1, 0);
        std::vector<int> q;
        q.reserve(n + 1);
        int head = 0;
        q.push_back(0);
        visited[0] = 1;

        while (head < static_cast<int>(q.size())) {
            int start = q[head];
            head++;
            for (int end = start + 1; end <= n; end++) {
                if (visited[end]) { continue; }
                if (words.count(s.substr(start, end - start))) {
                    if (end == n) { return true; }
                    visited[end] = 1;
                    q.push_back(end);
                }
            }
        }
        return false;
    }
};

// trie over hash-set: trie O(n) prefix matching per position, avoid substr allocation
// string_view (C++17) over substr: zero-copy, words stores views into wordDict, wordDict must outlive words
