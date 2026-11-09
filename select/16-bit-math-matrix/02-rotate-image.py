# LC 48 Rotate Image (Medium). Pattern: transpose + reverse rows.
# Rotate an n x n matrix 90 degrees clockwise, in place.


# Transpose across the main diagonal, then reverse every row. T: O(n^2), S: O(1).
def rotate(matrix):
    n = len(matrix)
    for r in range(n):
        # c starts past the diagonal so each pair swaps once
        for c in range(r + 1, n):
            matrix[r][c], matrix[c][r] = matrix[c][r], matrix[r][c]
    for row in matrix:
        row.reverse()


def test(label, matrix, want):
    rotate(matrix)
    print(label, "PASS" if matrix == want else "FAIL")


def main():
    test("one", [[7]], [[7]])
    test("two", [[1, 2], [3, 4]], [[3, 1], [4, 2]])
    test("three", [[1, 2, 3], [4, 5, 6], [7, 8, 9]], [[7, 4, 1], [8, 5, 2], [9, 6, 3]])


main()

# follow-up: counterclockwise -> transpose, then reverse each column (or reverse the
#   row order) instead of each row
# follow-up: one pass -> rotate ring by ring, cycling 4 cells at a time with a temp:
#   (r, c) <- (n-1-c, r) <- (n-1-r, n-1-c) <- (c, n-1-r) <- (r, c)
