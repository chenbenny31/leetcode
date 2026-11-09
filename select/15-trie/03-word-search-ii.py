# LC 212 Word Search II (Hard). Pattern: trie + grid backtracking.
# Return every word from words that can be traced on the board through 4-adjacent
# cells, each cell used at most once per word. Any order.


class TrieNode:
    def __init__(self):
        # children[c] = node for letter 'a' + c, None = none
        self.children = [None] * 26
        # the word ending here, "" if none; cleared once found so it is reported once
        self.word = ""


# extend the path to (r, c) if the trie has that letter; collect words that end here
def dfs(board, r, c, node, found):
    m = len(board)
    n = len(board[0])
    if r < 0 or r >= m or c < 0 or c >= n or board[r][c] == "#":
        return
    ch = board[r][c]
    nxt = node.children[ord(ch) - ord("a")]
    if nxt is None:
        return
    if nxt.word != "":
        found.append(nxt.word)
        nxt.word = ""
    # '#' marks the cell as used on the current path; restored on the way back
    board[r][c] = "#"
    dfs(board, r + 1, c, nxt, found)
    dfs(board, r - 1, c, nxt, found)
    dfs(board, r, c + 1, nxt, found)
    dfs(board, r, c - 1, nxt, found)
    board[r][c] = ch


# Trie of all words + one DFS per start cell that walks board and trie in step, so a
# dead prefix stops at once. T: O(m * n * 4 * 3^(L-1)), S: O(total letters).
def find_words(board, words):
    root = TrieNode()
    for word in words:
        cur = root
        for ch in word:
            letter = ord(ch) - ord("a")
            if cur.children[letter] is None:
                cur.children[letter] = TrieNode()
            cur = cur.children[letter]
        cur.word = word
    found = []
    m = len(board)
    n = len(board[0])
    for r in range(m):
        for c in range(n):
            dfs(board, r, c, root, found)
    return found


def test(label, board, words, want):
    got = find_words(board, words)
    got.sort()
    print(label, "PASS" if got == want else "FAIL")


def main():
    # c a
    # s t
    test("prefix-words", [["c", "a"], ["s", "t"]], ["cat", "cats", "tac", "cac"],
         ["cat", "cats", "tac"])
    test("cell-reuse", [["a", "b"]], ["aba"], [])
    test("report-once", [["a", "a"], ["a", "a"]], ["aaa"], ["aaa"])


main()

# follow-up: prune finished branches -> keep a child count per node; after the DFS of
#   a leaf with no word, set it to None in its parent so later starts skip it
# follow-up: single word (LC 79) -> same DFS with an index into the word, no trie
