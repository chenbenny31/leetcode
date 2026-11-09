# LC 124 Binary Tree Maximum Path Sum (Hard). Pattern: post-order gain + global best.
# A path is any chain of adjacent nodes, each used once, not necessarily through the
# root. Return the max path sum; the tree is non-empty and values may be negative.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# Every path bends at one node: try both sides at each node. T: O(n), S: O(h).
def max_path_sum(root):
    # best path bending anywhere so far
    best = root.val

    # returns the best downward chain from node
    def dfs(node):
        nonlocal best
        if node is None:
            return 0
        # a negative branch is better dropped
        left_gain = max(dfs(node.left), 0)
        right_gain = max(dfs(node.right), 0)
        best = max(best, node.val + left_gain + right_gain)
        # a parent can extend only one side
        return node.val + max(left_gain, right_gain)

    dfs(root)
    return best


def test(label, root, want):
    print(label, "PASS" if max_path_sum(root) == want else "FAIL")


def main():
    # -3(4(6, 7), 5)
    a6 = TreeNode(6)
    a7 = TreeNode(7)
    a4 = TreeNode(4, a6, a7)
    a5 = TreeNode(5)
    a3 = TreeNode(-3, a4, a5)
    test("bend-below-root", a3, 17)
    # 2(-1(4), 3)
    b4 = TreeNode(4)
    b1 = TreeNode(-1, b4)
    b3 = TreeNode(3)
    b2 = TreeNode(2, b1, b3)
    test("through-negative", b2, 8)
    c1 = TreeNode(-1)
    c2 = TreeNode(-2)
    c3 = TreeNode(-3, c1, c2)
    test("all-negative", c3, -1)


main()

# follow-up: diameter (LC 543) -> same skeleton on edge counts: best = max(best, l + r),
#   return 1 + max(l, r)
# follow-up: return the path -> record the best bend node, then follow larger gains down
