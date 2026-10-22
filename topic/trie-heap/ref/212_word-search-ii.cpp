// LC 212 Word Search II: given a character board and a word list, return every word that
// can be traced through horizontally/vertically adjacent cells, each cell used once.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

struct TrieNode {
    int child[26] = {};   // child[c] = pool index of the next node, 0 = none
    int word = -1;        // index into words of the word ending here, -1 = none
};

// Trie of all words, then one grid DFS per start cell that walks the trie in step so a
// dead prefix is abandoned at once. T: O(R*C*4*3^(L-1)) worst case, S: O(total letters).
void dfs(vector<vector<char>>& board, int r, int c, int node, vector<TrieNode>& pool,
         const vector<string>& words, vector<string>& found) {
    int m = board.size(), n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] == '#') { return; }
    int nxt = pool[node].child[board[r][c] - 'a'];
    if (nxt == 0) { return; }
    if (pool[nxt].word != -1) {
        found.push_back(words[pool[nxt].word]);
        // clear it so the same word is reported once
        pool[nxt].word = -1;
    }
    char letter = board[r][c];
    // '#' marks the cell as used on the current path
    board[r][c] = '#';
    dfs(board, r + 1, c, nxt, pool, words, found);
    dfs(board, r - 1, c, nxt, pool, words, found);
    dfs(board, r, c + 1, nxt, pool, words, found);
    dfs(board, r, c - 1, nxt, pool, words, found);
    board[r][c] = letter;
}

vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
    int m = board.size(), n = board[0].size();
    int total = words.size();
    vector<TrieNode> pool(1);   // node 0 is the root
    for (int i = 0; i < total; i++) {
        int cur = 0;
        for (char ch : words[i]) {
            if (pool[cur].child[ch - 'a'] == 0) {
                // push_back may reallocate, so take the index before using pool[cur]
                int fresh = pool.size();
                pool.push_back(TrieNode());
                pool[cur].child[ch - 'a'] = fresh;
            }
            cur = pool[cur].child[ch - 'a'];
        }
        pool[cur].word = i;
    }
    vector<string> found;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            dfs(board, r, c, 0, pool, words, found);
        }
    }
    return found;
}

void test(const string& label, vector<vector<char>> board, vector<string> words,
          vector<string> want) {
    vector<string> got = findWords(board, words);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("prefix-words", {{'c', 'a'}, {'s', 't'}}, {"cat", "cats", "tac", "cac"},
         {"cat", "cats", "tac"});
    test("cell-reuse", {{'a', 'b'}}, {"aba"}, {});
    test("duplicate-paths", {{'a', 'a'}, {'a', 'a'}}, {"aaa"}, {"aaa"});
    test("no-words", {{'z'}}, {}, {});
}
