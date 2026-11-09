# LC 98 Validate Binary Search Tree (Medium). Pattern: DFS with (lo, hi) bounds.
# True if every left subtree holds only smaller values and every right subtree only
# larger values (strict), at every node.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# each node must lie strictly inside (lo, hi) set by its ancestors
def dfs(node, lo, hi):
    if node is None:
        return True
    if node.val <= lo or node.val >= hi:
        return False
    return dfs(node.left, lo, node.val) and dfs(node.right, node.val, hi)


# Pass bounds down; infinite bounds, so any int value (even 2^31 - 1) fits inside.
# T: O(n), S: O(h).
def is_valid_bst(root):
    return dfs(root, float("-inf"), float("inf"))


def test(label, root, want):
    print(label, "PASS" if is_valid_bst(root) == want else "FAIL")


def main():
    a2 = TreeNode(2)
    a8 = TreeNode(8)
    a5 = TreeNode(5, a2, a8)
    test("valid", a5, True)
    # 5(2(_, 6), 8): 6 is right of 2 but left of 5
    b6 = TreeNode(6)
    b2 = TreeNode(2, None, b6)
    b8 = TreeNode(8)
    b5 = TreeNode(5, b2, b8)
    test("deep-violation", b5, False)
    c2 = TreeNode(2)
    c2b = TreeNode(2, c2)
    test("duplicate", c2b, False)
    d = TreeNode(2**31 - 1)
    test("int-max", d, True)


main()

# follow-up: inorder alternative -> inorder of a BST is strictly increasing; keep the
#   previous node's value and fail on prev >= cur
