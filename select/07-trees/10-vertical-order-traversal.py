# LC 987 Vertical Order Traversal (Hard). Pattern: collect (col, row, val), sort, group.
# Root at (row 0, col 0); left child (row + 1, col - 1), right child (row + 1, col + 1).
# Return columns left to right, each top to bottom; same row and column -> smaller first.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# record (col, row, val) for every node
def dfs(node, row, col, cells):
    if node is None:
        return
    cells.append((col, row, node.val))
    dfs(node.left, row + 1, col - 1, cells)
    dfs(node.right, row + 1, col + 1, cells)


# Sorting the tuples gives exactly the required order: column, then row, then value.
# T: O(n log n), S: O(n).
def vertical_traversal(root):
    # (col, row, val) of every node
    cells = []
    dfs(root, 0, 0, cells)
    cells.sort()

    result = []
    prev_col = 0
    for i in range(len(cells)):
        col, row, val = cells[i]
        # a new column starts a new group
        if i == 0 or col != prev_col:
            result.append([])
        result[-1].append(val)
        prev_col = col
    return result


def test(label, root, want):
    print(label, "PASS" if vertical_traversal(root) == want else "FAIL")


def main():
    # 1(2(4, 6), 3(5, 7)): 6 and 5 share row 2, column 0
    n4 = TreeNode(4)
    n6 = TreeNode(6)
    n5 = TreeNode(5)
    n7 = TreeNode(7)
    n2 = TreeNode(2, n4, n6)
    n3 = TreeNode(3, n5, n7)
    n1 = TreeNode(1, n2, n3)
    test("tie-by-value", n1, [[4], [2], [1, 5, 6], [3], [7]])
    solo = TreeNode(9)
    test("single", solo, [[9]])
    test("empty", None, [])


main()

# follow-up: LC 314 (same row -> left to right, no value sort) -> BFS with column, append
#   in visit order to a dict col -> list, output in sorted(col) order
# follow-up: avoid the full sort -> dict col -> list of (row, val), sort each column only
