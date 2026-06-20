// dfs + memoization, T: O(n^2 * 2^n), S: O(n2^n)

#include <string>
#include <string_view> // C++17
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Solution {
public:
    std::vector<std::string> wordBreak(std::string s, std::vector<std::string>& wordDict) {
        std::unordered_set<std::string_view> words(wordDict.begin(), wordDict.end());
        std::unordered_map<int, std::vector<std::string>> memo;
        std::string_view sv(s);
        return dfs(sv, words, memo, 0);
    }

private:
    std::vector<std::string> dfs(std::string_view sv, std::unordered_set<std::string_view>& words,
                                 std::unordered_map<int, std::vector<std::string>>& memo, int start) {
        auto it = memo.find(start);
        if (it != memo.end()) { return it->second; }

        std::vector<std::string> res;
        if (start == static_cast<int>(sv.size())) {
            res.push_back("");
            return res;
        }

        for (int end = start + 1; end <= static_cast<int>(sv.size()); end++) {
            std::string_view word = sv.substr(start, end - start);
            if (!words.count(word)) { continue; }
            std::vector<std::string> rest = dfs(sv, words, memo, end);
            for (auto& sentence : rest) {
                std::string curr = std::string(word);
                if (!sentence.empty()) { curr += " " + sentence; }
                res.push_back(std::move(curr));
            }
        }
        memo[start] = res;
        return res;
    }
};

// trie + dfs + memoization, T: O(n * 2^n), S: O(wr), S: w = total chars, r: alpha_size = 26

#include <string>
#include <string_view> // C++17
#include <vector>
#include <unordered_map>
#include <cstring> // std::memset
#include <cstdint> // uint8_t

class Solution {
private:
    static constexpr int N = 10001;
    static constexpr char BASE = 'a';

    struct TrieNode {
        int children[26];
        bool is_end;
    };

    TrieNode pool[N];
    int cnt = 0;

    int new_node() {
        pool[cnt].is_end = false;
        std::memset(pool[cnt].children, -1, sizeof(pool[cnt].children));
        cnt++;
        return cnt - 1;
    }

    void insert(std::string_view word) {
        int cur = 0;
        for (char c : word) {
            int idx = c - BASE;
            if (pool[cur].children[idx] == -1) {
                pool[cur].children[idx] = new_node();
            }
            cur = pool[cur].children[idx];
        }
        pool[cur].is_end = true;
    }

    void dfs(std::string_view sv, int start,
             std::unordered_map<int, std::vector<std::string>>& memo,
             std::vector<std::string>& result) {
        auto it = memo.find(start);
        if (it != memo.end()) { result = it->second; return; }

        if (start == static_cast<int>(sv.size())) { result.push_back(""); return; }

        int cur = 0;
        for (int end = start; end < static_cast<int>(sv.size()); end++) {
            int idx = sv[end] - BASE;
            if (pool[cur].children[idx] == -1) { break; } // prune, no word with this
            cur = pool[cur].children[idx];
            if (!pool[cur].is_end) { continue; }

            std::vector<std::string> rest;
            dfs(sv, end + 1, memo, rest);
            std::string_view word = sv.substr(start, end - start + 1);
            for (auto& sentence : rest) {
                std::string curr = std::string(word);
                if (!sentence.empty()) { curr += " " + sentence; }
                result.push_back(std::move(curr));
            }
        }
        memo[start] = result;
    }

public:
    std::vector<std::string> wordBreak(std::string s, std::vector<std::string>& wordDict) {
        cnt = 0;
        new_node(); // root = node 0
        for (auto& w : wordDict) { insert(w); }

        std::unordered_map<int, std::vector<std::string>> memo;
        std::string_view sv(s);
        std::vector<std::string> result;
        dfs(sv, 0, memo, result);
        return result;
    }
};


// memo keyed by start index: cached after first computation, subsequent calls to same start return cached vector
// base case start == sv.size(): reach end of string
