# 22 · Set Matrix Zeroes (ServiceNow, experienced hires, Medium) — LC 73
# set the whole row and column of every 0 to 0, in place; then do it in O(1) extra space
#
# clarify:
# in place -> yes, return nothing
# zeros created by this step spread further? -> no, only the original zeros count
# empty matrix -> not given


# ==================== solution 1: row / column flags (start here) ====================
# record the zero rows and columns first, then clear; clearing while scanning would
# spread new zeros

# T: O(m * n), S: O(m + n)
def set_zeroes(matrix):
    m = len(matrix)
    n = len(matrix[0])
    # zero_row[r] = row r has a 0; zero_col[c] = column c has a 0
    zero_row = [False] * m
    zero_col = [False] * n
    for r in range(m):
        for c in range(n):
            if matrix[r][c] == 0:
                zero_row[r] = True
                zero_col[c] = True
    for r in range(m):
        for c in range(n):
            if zero_row[r] or zero_col[c]:
                matrix[r][c] = 0


# ==================== solution 2: O(1) space (follow-up) ====================
# row 0 and column 0 double as the flags for the other rows and columns; their own zero
# status is saved first

# T: O(m * n), S: O(1)
def set_zeroes_const_space(matrix):
    m = len(matrix)
    n = len(matrix[0])
    row0_zero = 0 in matrix[0]
    col0_zero = False
    for r in range(m):
        if matrix[r][0] == 0:
            col0_zero = True
    for r in range(1, m):
        for c in range(1, n):
            if matrix[r][c] == 0:
                matrix[r][0] = 0
                matrix[0][c] = 0
    for r in range(1, m):
        for c in range(1, n):
            if matrix[r][0] == 0 or matrix[0][c] == 0:
                matrix[r][c] = 0
    # row 0 and column 0 last, after they stopped serving as flags
    if row0_zero:
        for c in range(n):
            matrix[0][c] = 0
    if col0_zero:
        for r in range(m):
            matrix[r][0] = 0


# ==================== tests ====================

def test(label, matrix, want):
    first = [row[:] for row in matrix]
    second = [row[:] for row in matrix]
    set_zeroes(first)
    set_zeroes_const_space(second)
    ok = first == want and second == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("one-zero", [[1, 0, 1], [1, 1, 1]], [[0, 0, 0], [1, 0, 1]])
    test("corner", [[0, 1], [1, 1]], [[0, 0], [0, 1]])
    test("no-zero", [[1, 2], [3, 4]], [[1, 2], [3, 4]])


main()

# follow-up:
# 1. why save row 0 / column 0 first -> the flags written into them would look like zeros
# 2. game of life in place (LC 289) -> same trick: encode old and new state in one cell
