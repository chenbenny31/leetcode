// 73 · Word Search — LC 79
// Mother problem: select/12-backtracking/04-word-search.cpp
//
// LC 79 (Medium), grid DFS backtracking: does word appear in the board as a path of
// horizontally / vertically adjacent cells, using each cell at most once?
// Google reports: word search (1p3a)
//
// Clarify
// - modify the board? → mark with '#' and restore
// - board at least 1 x 1, word non-empty → assume
//
// Core idea
// - DFS from every cell; at a matching cell mark it '#', try 4 neighbors for the next
//   letter, restore before returning; true when pos reaches the last letter.
// - T O(m * n * 3^L), L = word length; S O(L) recursion.
//
// Tests: snake path, needs a cell twice, missing letter, single cell
//
// Follow-ups
// - many words (LC 212) → trie of the words, one DFS per cell walks the trie; remove
//   found words to prune
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// does word[pos..] start at cell (r, c)?
bool dfs(vector<vector<char>>& board, const string& word, int r, int c, int pos) {
    int m = board.size();
    int n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != word[pos]) { return false; }
    if (pos == (int)word.size() - 1) { return true; }
    char letter = board[r][c];
    // '#' marks the cell as used on the current path
    board[r][c] = '#';
    int nextPos = pos + 1;
    bool found = dfs(board, word, r + 1, c, nextPos) ||
                 dfs(board, word, r - 1, c, nextPos) ||
                 dfs(board, word, r, c + 1, nextPos) ||
                 dfs(board, word, r, c - 1, nextPos);
    // restore before returning, found or not
    board[r][c] = letter;
    return found;
}

// DFS from every cell. T: O(m * n * 3^L) for word length L, S: O(L) recursion.
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
