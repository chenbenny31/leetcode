# LC 79 Word Search (Medium). Pattern: grid DFS backtracking, mark cell in place.
# Does word appear in the board as a path of horizontally / vertically adjacent cells,
# using each cell at most once?


# does word[pos..] start at cell (r, c)?
def dfs(board, word, r, c, pos):
    m = len(board)
    n = len(board[0])
    if r < 0 or r >= m or c < 0 or c >= n:
        return False
    if board[r][c] != word[pos]:
        return False
    if pos == len(word) - 1:
        return True
    letter = board[r][c]
    # '#' marks the cell as used on the current path
    board[r][c] = "#"
    next_pos = pos + 1
    found = (dfs(board, word, r + 1, c, next_pos)
             or dfs(board, word, r - 1, c, next_pos)
             or dfs(board, word, r, c + 1, next_pos)
             or dfs(board, word, r, c - 1, next_pos))
    # restore before returning, found or not
    board[r][c] = letter
    return found


# DFS from every cell. T: O(m * n * 3^L) for word length L, S: O(L) recursion.
def exist(board, word):
    m = len(board)
    n = len(board[0])
    for r in range(m):
        for c in range(n):
            if dfs(board, word, r, c, 0):
                return True
    return False


def test(label, board, word, want):
    print(label, "PASS" if exist(board, word) == want else "FAIL")


def main():
    test("snake-path", [["a", "b"], ["d", "c"]], "abcd", True)
    test("cell-reuse", [["a", "b"]], "aba", False)
    test("missing-letter", [["a", "b"], ["c", "d"]], "az", False)
    test("single-cell", [["q"]], "q", True)


main()

# follow-up: many words (LC 212) -> build a trie of the words, one DFS per cell walks
#   the trie; remove found words to prune
