// dp + hash-set

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cstdint>
#include <string_view>

class Solution {
public:
    bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
        const int n = static_cast<int>(s.length());

        // std::unordered_set<std::string> words(wordDict.begin(), wordDict.end());
        std::unordered_set<std::string_view> words;
        words.reserve(wordDict.size());
        words.max_load_factor(0.25f);

        int maxWordLen = 0; // help for pruning
        for (const std::string& w : wordDict) {
            words.insert(std::string_view(w));
            maxWordLen = std::max(maxWordLen, static_cast<int>(w.length()));
        }

        std::string_view sv(s);
        std::vector<uint8_t> dp(n + 1, 0);
        dp[0] = 1;

        for (int i = 1; i <= n; i++) {
            for (int j = i - 1; j >= 0 && i - j <= maxWordLen; j--) {
                // if (dp[j] && words.count(s.substr(j, i - j)) > 0) {
                if (dp[j] && words.count(sv.substr(j, i - j)) > 0) {
                    dp[i] = 1;
                    break;
                }
            }
        }
        return dp[n];
    }
};

// dp with trie

class Solution {
private:
    struct TrieNode {
        TrieNode* children[26] = {};
        bool isWord = false;
    };

    TrieNode* root;

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isWord = true;
    }

public:
    bool wordBreak(std::string s, std::vector<std::string>& wordDict) {
        root = new TrieNode(); 
        for (const std::string& w : wordDict) { insert(w); }

        const int n = static_cast<int>(s.length());
        std::vector<uint8_t> dp(n + 1, false);
        dp[n] = 1;

        for (int i = n - 1; i >= 0; i--) { // backwards to avoid dp[i] reads dp[j+1] where j >= i, higher indices must be finalized
            TrieNode* node = root;
            for (int j = i ; j < n; j++) {
                int idx = s[j] - 'a';
                if (!node->children[idx]) { break; }
                node = node->children[idx];
                if (node->isWord && dp[j + 1]) {
                    dp[i] = 1;
                    break;
                }
            }
        }
        return dp[0];
    }
};
