// LC 79 Word Search: does word appear in the character board as a path of horizontally
// or vertically adjacent cells, using each cell at most once?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// does the path for word[i..] exist starting at cell (r, c)?
bool dfs(vector<vector<char>>& board, const string& word, int r, int c, int i) {
    int m = board.size(), n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] != word[i]) { return false; }
    if (i + 1 == (int)word.size()) { return true; }
    char letter = board[r][c];
    // '#' marks the cell as used on the current path
    board[r][c] = '#';
    bool found = dfs(board, word, r + 1, c, i + 1) || dfs(board, word, r - 1, c, i + 1) ||
                 dfs(board, word, r, c + 1, i + 1) || dfs(board, word, r, c - 1, i + 1);
    board[r][c] = letter;
    return found;
}

// Backtracking DFS from every cell. T: O(R*C*3^L) for word length L, S: O(L).
bool exist(vector<vector<char>>& board, string word) {
    int m = board.size(), n = board[0].size();
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
