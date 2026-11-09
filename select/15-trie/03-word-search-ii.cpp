// LC 212 Word Search II (Hard). Pattern: trie + grid backtracking.
// Return every word from words that can be traced on the board through 4-adjacent
// cells, each cell used at most once per word. Any order.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

struct TrieNode {
    // children[c] = node for letter 'a' + c, nullptr = none
    TrieNode* children[26] = {};
    // the word ending here, "" if none; cleared once found so it is reported once
    string word;

    ~TrieNode() {
        for (TrieNode* child : children) {
            delete child;
        }
    }
};

// extend the path to (r, c) if the trie has that letter; collect words that end here
void dfs(vector<vector<char>>& board, int r, int c, TrieNode* node,
         vector<string>& found) {
    int m = board.size();
    int n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || board[r][c] == '#') { return; }
    char ch = board[r][c];
    TrieNode* nxt = node->children[ch - 'a'];
    if (nxt == nullptr) { return; }
    if (!nxt->word.empty()) {
        found.push_back(nxt->word);
        nxt->word.clear();
    }
    // '#' marks the cell as used on the current path; restored on the way back
    board[r][c] = '#';
    dfs(board, r + 1, c, nxt, found);
    dfs(board, r - 1, c, nxt, found);
    dfs(board, r, c + 1, nxt, found);
    dfs(board, r, c - 1, nxt, found);
    board[r][c] = ch;
}

// Trie of all words + one DFS per start cell that walks board and trie in step, so a
// dead prefix stops at once. T: O(m * n * 4 * 3^(L-1)), S: O(total letters).
vector<string> findWords(vector<vector<char>>& board, const vector<string>& words) {
    TrieNode root;
    for (const string& word : words) {
        TrieNode* cur = &root;
        for (char ch : word) {
            int letter = ch - 'a';
            if (cur->children[letter] == nullptr) {
                cur->children[letter] = new TrieNode();
            }
            cur = cur->children[letter];
        }
        cur->word = word;
    }
    vector<string> found;
    int m = board.size();
    int n = board[0].size();
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            dfs(board, r, c, &root, found);
        }
    }
    return found;
}

void test(const string& label, vector<vector<char>> board, const vector<string>& words,
          const vector<string>& want) {
    vector<string> got = findWords(board, words);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // c a
    // s t
    test("prefix-words", {{'c', 'a'}, {'s', 't'}}, {"cat", "cats", "tac", "cac"},
         {"cat", "cats", "tac"});
    test("cell-reuse", {{'a', 'b'}}, {"aba"}, {});
    test("report-once", {{'a', 'a'}, {'a', 'a'}}, {"aaa"}, {"aaa"});
}

// follow-up: prune finished branches -> keep a child count per node; after the DFS of
//   a leaf with no word, delete it from its parent so later starts skip it
// follow-up: single word (LC 79) -> same DFS with an index into the word, no trie
