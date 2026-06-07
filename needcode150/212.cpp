// trie + backtracking, T: O(mn4^(mn)), S: O(total chars in words)

#include <vector>
#include <string>
#include <cstring>

class Solution {
private:
    static constexpr int MAX = 10001;
    static constexpr char BASE = 'a';

    struct TrieNode {
        int children[26];
        int word_idx; // -1 if not end, else index into words
    };

    TrieNode pool[MAX];
    int cnt;

    int new_node() {
        pool[cnt].word_idx = -1;
        std::memset(pool[cnt].children, -1, sizeof(pool[cnt].children));
        cnt++;
        return cnt - 1;
    }

    void build_trie(std::vector<std::string>& words) {
        cnt = 0; new_node();
        for (int wi = 0; wi < static_cast<int>(words.size()); wi++) {
            int cur = 0;
            for (char c : words[wi]) {
                int idx = c - BASE;
                if (pool[cur].children[idx] == -1) { pool[cur].children[idx] = new_node(); }
                cur = pool[cur].children[idx];
            }
            pool[cur].word_idx = wi;
        }
    }

    void dfs(std::vector<std::vector<char>>& board,
             int r, int c, int node,
             std::vector<std::string>& words,
             std::vector<std::string>& out) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());

        if (r < 0 || r >= m || c < 0 || c >= n) { return; }
        char ch = board[r][c];
        if (ch == '#') { return; }

        int idx = ch - BASE;
        int succ = pool[node].children[idx];
        if (succ == -1) { return; }

        if (pool[succ].word_idx != -1) {
            out.push_back(words[pool[succ].word_idx]);
            pool[succ].word_idx = -1;
        }

        board[r][c] = '#';
        dfs(board, r + 1, c, succ, words, out);
        dfs(board, r - 1, c, succ, words, out);
        dfs(board, r, c + 1, succ, words, out);
        dfs(board, r, c - 1, succ, words, out);
        board[r][c] = ch;
    }

public:
    std::vector<std::string> findWords(std::vector<std::vector<char>>& board,
                                       std::vector<std::string>& words) {
        build_trie(words);
        std::vector<std::string> out;
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                dfs(board, r, c, 0, words, out);
            }
        }
        return out;
    }
};

// word_idx not is_end: directly retrieve found word without hash-map lookup, -1 sentinel
// pool[succ].word_idx = -1: deduplication same word found via multiple paths
// trie pruning: succ = -1, no word in trie starts with this prefix
