# LC 437 Path Sum III (Medium). Pattern: prefix sums on the root path + backtracking.
# Count downward paths (parent to child, any start, any end) whose values sum to
# targetSum. Values may be negative.


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# prefix = sum root..node; a path ending here hits the target if it starts just below
# an ancestor whose prefix is prefix - target
def dfs(node, prefix, target, prefix_count):
    if node is None:
        return 0
    prefix += node.val
    count = prefix_count.get(prefix - target, 0)
    prefix_count[prefix] = prefix_count.get(prefix, 0) + 1
    count += dfs(node.left, prefix, target, prefix_count)
    count += dfs(node.right, prefix, target, prefix_count)
    # backtrack: the map must hold only the current root path
    prefix_count[prefix] -= 1
    return count


# Subarray-sum-equals-k on every root-to-node path. T: O(n), S: O(h).
def path_sum(root, target_sum):
    # prefix sum -> times it occurs on the current root path
    prefix_count = {}
    # the empty prefix, for paths starting at the root
    prefix_count[0] = 1
    return dfs(root, 0, target_sum, prefix_count)


def test(label, root, target, want):
    print(label, "PASS" if path_sum(root, target) == want else "FAIL")


def main():
    # 5(3(2), -2(_, 4))
    n2 = TreeNode(2)
    n4 = TreeNode(4)
    n3 = TreeNode(3, n2)
    nm2 = TreeNode(-2, None, n4)
    n5 = TreeNode(5, n3, nm2)
    test("not-from-root", n5, 5, 2)
    test("with-negative", n5, 3, 2)
    m1 = TreeNode(-1)
    p1 = TreeNode(1, m1)
    test("zero-sum", p1, 0, 1)
    test("empty", None, 1, 0)


main()

# follow-up: brute force first -> from every node DFS down summing, O(n * h)
# follow-up: array version (LC 560) -> same prefix-count map over one pass of the array
