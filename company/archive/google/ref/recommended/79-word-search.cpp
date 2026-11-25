// 79 · Word Search — LC 79
// Mother: select/12-backtracking/04-word-search.cpp
// is word a path of 4-adjacent cells in board, each cell used at most once?
// reported: word search (1p3a)
//
// clarify:
// may modify board -> yes, mark and restore
// board size -> >= 1 x 1
// word -> non-empty
// many words -> follow-up (LC 212)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// true if word[pos..] can be traced from cell (r, c)
bool dfs(vector<vector<char>>& board, const string& word, int r, int c, int pos) {
    int m = board.size();
    int n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != word[pos]) { return false; }
    if (pos == (int)word.size() - 1) { return true; }
    char letter = board[r][c];
    // '#' = used on current path; restored before return
    board[r][c] = '#';
    int nextPos = pos + 1;
    bool found = dfs(board, word, r + 1, c, nextPos) ||
                 dfs(board, word, r - 1, c, nextPos) ||
                 dfs(board, word, r, c + 1, nextPos) ||
                 dfs(board, word, r, c - 1, nextPos);
    board[r][c] = letter;
    return found;
}

// core idea: DFS from every cell; mark matching cell, try 4 neighbors, unmark on return
// T: O(m * n * 3^L), S: O(L) — L = word len; 3 = no stepping back
bool exist(vector<vector<char>>& board, const string& word) {
    int m = board.size();
    int n = board[0].size();
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (dfs(board, word, r, c, 0)) { return true; }
        }
    }
    return false;
}

void test(const string& label, vector<vector<char>> board, const string& word,
          bool want) {
    cout << label << (exist(board, word) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("snake-path", {{'a', 'b'}, {'d', 'c'}}, "abcd", true);
    test("cell-reuse", {{'a', 'b'}}, "aba", false);
    test("missing-letter", {{'a', 'b'}, {'c', 'd'}}, "az", false);
    test("single-cell", {{'q'}}, "q", true);
}

// follow-up:
// many words (LC 212) -> trie of words, one DFS per cell walks the trie
// LC 212 pruning -> remove found words from the trie
