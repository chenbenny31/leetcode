// trie with dfs for wildcard, T: O(m) insert, O(26^m) worst case with all ., S: O(26mn)

#include <cstring> // std::memset
#include <string>

class WordDictionary {
private:
    static constexpr int MAX = 100001;
    static constexpr char BASE = 'a';

    struct TrieNode {
        int children[26];
        bool is_end;
    };

    TrieNode pool[MAX];
    int cnt;

    int new_node() {
        pool[cnt].is_end = false;
        std::memset(pool[cnt].children, -1, sizeof(pool[cnt].children));
        cnt++;
        return cnt - 1;
    }

    bool dfs(const std::string& word, int wi, int node) {
        if (wi == static_cast<int>(word.size())) { return pool[node].is_end; }
        char c = word[wi];
        if (c == '.') {
            for (int i = 0; i < 26; i++) {
                if (pool[node].children[i] != -1 && dfs(word, wi + 1, pool[node].children[i])) { return true; }
            }
            return false;
        }
        int idx = c - BASE;
        if (pool[node].children[idx] == -1) { return false; }
        return dfs(word, wi + 1, pool[node].children[idx]);
    }

public:
    WordDictionary() { cnt = 0; new_node(); }

    void addWord(const std::string& word) {
        int cur = 0;
        for (char c : word) {
            int idx = c - BASE;
            if (pool[cur].children[idx] == -1) { pool[cur].children[idx] = new_node(); }
            cur = pool[cur].children[idx];
        }
        pool[cur].is_end = true;
    }

    bool search(const std::string& word) {
        return dfs(word, 0, 0);
    }
};

// wildcard dfs branching factor: . tries all 26 children - O(26^d), d = number of dots
// pool allocation: MAX = max_words * max_word_length + 1 (root node)
// iterative version: explicit stack of {node_idx, word_idx}, recursive preferred here due to bounded depth = word length <= 25
