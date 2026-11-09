# LC 863 All Nodes Distance K in Binary Tree (Medium). Pattern: tree -> graph, then BFS.
# Values are unique. Given a target node and k, return the values of all nodes exactly
# k edges from the target, in any order.

from collections import defaultdict
from collections import deque


class TreeNode:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right


# adds an undirected edge between each node and each of its children
def dfs(node, graph):
    if node is None:
        return
    if node.left:
        graph[node.val].append(node.left.val)
        graph[node.left.val].append(node.val)
        dfs(node.left, graph)
    if node.right:
        graph[node.val].append(node.right.val)
        graph[node.right.val].append(node.val)
        dfs(node.right, graph)


# With parent edges the tree is an undirected graph: BFS k levels out from the target.
# T: O(n), S: O(n).
def distance_k(root, target, k):
    # graph[v] = values of v's parent and children (edge = tree edge)
    graph = defaultdict(list)
    dfs(root, graph)
    # nodes at the current distance from target
    q = deque([target.val])
    # nodes already reached
    seen = {target.val}
    dist = 0
    while dist < k and q:
        level_size = len(q)
        for _ in range(level_size):
            cur = q.popleft()
            for nxt in graph[cur]:
                if nxt in seen:
                    continue
                seen.add(nxt)
                q.append(nxt)
        dist += 1
    return list(q)


def test(label, root, target, k, want):
    got = sorted(distance_k(root, target, k))
    print(label, "PASS" if got == want else "FAIL")


def main():
    # 1(2(4, 5), 3(6, 7))
    n4 = TreeNode(4)
    n5 = TreeNode(5)
    n6 = TreeNode(6)
    n7 = TreeNode(7)
    n2 = TreeNode(2, n4, n5)
    n3 = TreeNode(3, n6, n7)
    n1 = TreeNode(1, n2, n3)
    test("k-zero", n1, n2, 0, [2])
    test("up-and-down", n1, n2, 1, [1, 4, 5])
    test("across-root", n1, n4, 3, [3])
    test("too-far", n1, n2, 5, [])


main()

# follow-up: parent map only -> dict child node -> parent node, BFS over left / right /
#   parent with a seen set
# follow-up: closest leaf to target (LC 742) -> same graph, BFS until a leaf is popped
