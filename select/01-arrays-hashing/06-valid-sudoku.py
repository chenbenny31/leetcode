# LC 36 Valid Sudoku (Medium). Pattern: seen flags per row, column, box.
# Is a partially filled 9x9 board valid? Each row, column and 3x3 box must hold digits
# 1-9 without repeats; '.' is empty. Solvability is not required.


# One pass; each digit marks its row, column and box. T: O(81), S: O(81).
def is_valid_sudoku(board):
    # rows[r][digit] = digit already in row r (digit 0..8); same for cols and boxes
    rows = [[False] * 9 for _ in range(9)]
    cols = [[False] * 9 for _ in range(9)]
    boxes = [[False] * 9 for _ in range(9)]
    for r in range(9):
        for c in range(9):
            if board[r][c] == ".":
                continue
            # "1".."9" -> 0..8
            digit = int(board[r][c]) - 1
            # box index 0..8, row-major
            box = (r // 3) * 3 + c // 3
            if rows[r][digit] or cols[c][digit] or boxes[box][digit]:
                return False
            rows[r][digit] = True
            cols[c][digit] = True
            boxes[box][digit] = True
    return True


# cells = [r, c, digit] placed on an otherwise empty board
def test(label, cells, want):
    board = [["."] * 9 for _ in range(9)]
    for r, c, digit in cells:
        board[r][c] = str(digit)
    got = is_valid_sudoku(board)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("ok", [[0, 0, 5], [4, 4, 5]], True)
    test("row", [[0, 0, 5], [0, 8, 5]], False)
    test("col", [[0, 0, 5], [8, 0, 5]], False)
    test("box", [[0, 0, 5], [1, 1, 5]], False)


main()

# follow-up: less memory -> one int bitmask per row / column / box, bit d = digit seen
# follow-up: solve it (LC 37) -> backtracking over empty cells with the same three
#   tables: place a digit, recurse, unmark on the way back
