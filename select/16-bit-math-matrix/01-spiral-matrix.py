# LC 54 Spiral Matrix (Medium). Pattern: four shrinking bounds.
# Return all elements of a non-empty m x n matrix in clockwise spiral order, starting
# at the top-left.


# Bounds of the rectangle not yet visited; peel one ring per lap.
# T: O(m * n), S: O(1) besides the output.
def spiral_order(matrix):
    m = len(matrix)
    n = len(matrix[0])
    top = 0
    bottom = m - 1
    left = 0
    right = n - 1
    result = []
    while top <= bottom and left <= right:
        for c in range(left, right + 1):
            result.append(matrix[top][c])
        top += 1

        for r in range(top, bottom + 1):
            result.append(matrix[r][right])
        right -= 1

        # a single remaining row was already read left to right
        if top <= bottom:
            for c in range(right, left - 1, -1):
                result.append(matrix[bottom][c])
            bottom -= 1

        # a single remaining column was already read top to bottom
        if left <= right:
            for r in range(bottom, top - 1, -1):
                result.append(matrix[r][left])
            left += 1
    return result


def test(label, matrix, want):
    got = spiral_order(matrix)
    print(label, "PASS" if got == want else "FAIL")


def main():
    test("one-row", [[1, 2, 3]], [1, 2, 3])
    test("one-col", [[1], [2], [3]], [1, 2, 3])
    test("tall", [[1, 2], [3, 4], [5, 6], [7, 8]], [1, 2, 4, 6, 8, 7, 5, 3])
    test("wide-center-row", [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12]],
         [1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7])


main()

# follow-up: spiral matrix II (LC 59) -> same four bounds, write val, val += 1 instead
#   of read
# follow-up: direction-array version -> dr/dc arrays + visited, turn right when the next
#   cell is out of range or visited; O(m * n) extra space
