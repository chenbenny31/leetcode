# LC 51 N-Queens (Hard). Pattern: row-by-row backtracking with column/diagonal flags.
# Return all ways to place n queens on an n x n board so that no two attack each
# other ('Q' = queen, '.' = empty).


# places a queen in row r, then recurses on row r + 1
def dfs(r, n, board, cols, diag, anti, result):
    if r == n:
        # rows are char lists (strings are immutable); join a snapshot of each
        result.append(["".join(row) for row in board])
        return
    for c in range(n):
        # r + c is constant on a '/' diagonal, r - c on a '\' one (shifted by n - 1)
        diag_id = r + c
        anti_id = r - c + n - 1
        if cols[c] or diag[diag_id] or anti[anti_id]:
            continue
        cols[c] = True
        diag[diag_id] = True
        anti[anti_id] = True
        board[r][c] = "Q"
        dfs(r + 1, n, board, cols, diag, anti, result)
        board[r][c] = "."
        cols[c] = False
        diag[diag_id] = False
        anti[anti_id] = False


# One queen per row; O(1) conflict checks via flags. T: O(n!), S: O(n^2).
def solve_n_queens(n):
    # current partial placement, one char list per row
    board = [["."] * n for _ in range(n)]
    # cols[c] = column c taken
    cols = [False] * n
    # diag[r + c] = '/' diagonal taken
    diag = [False] * (2 * n - 1)
    # anti[r - c + n - 1] = '\' diagonal taken (shifted to be non-negative)
    anti = [False] * (2 * n - 1)
    result = []
    dfs(0, n, board, cols, diag, anti, result)
    return result


def test(label, n, want_count, want_first):
    got = solve_n_queens(n)
    ok = len(got) == want_count and (len(got) == 0 or got[0] == want_first)
    print(label, "PASS" if ok else "FAIL")


def main():
    test("none", 3, 0, [])
    test("five", 5, 10, ["Q....", "..Q..", "....Q", ".Q...", "...Q."])
    test("six", 6, 4, [".Q....", "...Q..", ".....Q", "Q.....", "..Q...", "....Q."])


main()

# follow-up: count only (LC 52) -> same dfs without the board, return the sum
# follow-up: bitmask flags -> avail = ~(cols | d | a) & ((1 << n) - 1); take the
#   lowest bit p = avail & -avail, next row d = (d | p) << 1, a = (a | p) >> 1
#   (Python ints are unbounded: the & mask keeps shifted bits from piling up)
