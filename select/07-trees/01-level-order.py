# LC 102 Binary Tree Level Order Traversal (Medium). Pattern: BFS by level size.
# Return the node values level by level, left to right.

from collections import deque


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# BFS; the queue size at the start of a level is that level's size. T: O(n), S: O(n).
def level_order(root):
    levels = []
    if root is None:
        return levels
    # nodes of the current level, then their children
    q = deque([root])
    while q:
        level_size = len(q)
        level = []
        for _ in range(level_size):
            cur = q.popleft()
            level.append(cur.val)
            if cur.left:
                q.append(cur.left)
            if cur.right:
                q.append(cur.right)
        levels.append(level)
    return levels


def test(label, root, want):
    print(label, "PASS" if level_order(root) == want else "FAIL")


def main():
    # 1(2(_, 4), 3(_, 5))
    n4 = TreeNode(4)
    n5 = TreeNode(5)
    n2 = TreeNode(2, None, n4)
    n3 = TreeNode(3, None, n5)
    n1 = TreeNode(1, n2, n3)
    test("three-levels", n1, [[1], [2, 3], [4, 5]])
    single = TreeNode(7)
    test("single", single, [[7]])
    test("empty", None, [])


main()

# follow-up: zigzag (LC 103) -> same BFS, reverse every other level before saving it
# follow-up: right side view (LC 199) -> keep only the last node of each level
