// LC 79 Word Search (Medium) — grid DFS backtracking, mark the cell in place
// return if word is a path of 4-adjacent cells in board, each cell used at most once
//
// clarify:
// 4 directions only -> no diagonals
// cell used once per path -> mark while on path, restore after
// case-sensitive -> compare chars as is
// board / word non-empty -> board[0] is safe
// may board be modified temporarily? -> yes; else visited matrix

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// true if word[pos..] starts at cell (r, c)
bool dfs(vector<vector<char>>& board, const string& word, int r, int c, int pos) {
    int m = board.size();
    int n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != word[pos]) { return false; }
    if (pos == (int)word.size() - 1) { return true; }
    char letter = board[r][c];
    // '#' marks cell as used on current path
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

// core idea: DFS from every cell, one letter per step; mark cell on path, restore after
// T: O(m * n * 3^L), S: O(L) — L = word len; 3 = directions minus the one we came from
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
// many words on one board (LC 212) -> trie of words, one DFS per cell walks the trie
// board must not be modified -> separate visited matrix instead of '#'
