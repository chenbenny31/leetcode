# 25 · Populating Next Right Pointers in Each Node (ServiceNow phone 2024, Medium)
# LC 116: in a perfect binary tree, set each node's next to its right neighbor on its
# level (None at the end of a level); then do it in O(1) extra space
#
# clarify:
# perfect tree -> every parent has two children, all leaves on one level
# empty tree -> return None
# O(1) space -> no queue; the recursion stack doesn't count on LC, but this has none


class Node:
    def __init__(self, val):
        self.val = val
        self.left = None
        self.right = None
        self.next = None


# core idea: walk a level along its already wired next chain and wire the level below
# T: O(n), S: O(1)
def connect(root):
    # first node of the level whose next chain is wired
    leftmost = root
    while leftmost and leftmost.left:
        cur = leftmost
        while cur:
            cur.left.next = cur.right
            # across a parent boundary: right child -> neighbor parent's left child
            if cur.next:
                cur.right.next = cur.next.left
            cur = cur.next
        leftmost = leftmost.left
    return root


# nodes = the tree in level order; want[i] = val of nodes[i].next, None at a level end
def test(label, nodes, want):
    if nodes:
        connect(nodes[0])
    else:
        connect(None)
    got = []
    for node in nodes:
        if node.next:
            got.append(node.next.val)
        else:
            got.append(None)
    ok = got == want
    print(label, "PASS" if ok else "FAIL")


def main():
    nodes = []
    for v in range(1, 8):
        nodes.append(Node(v * 10))
    # nodes[i] has children nodes[2i + 1] and nodes[2i + 2]
    for i in range(3):
        nodes[i].left = nodes[2 * i + 1]
        nodes[i].right = nodes[2 * i + 2]
    test("cross-parent", nodes, [None, 30, None, 50, 60, 70, None])
    test("single", [Node(9)], [None])
    test("empty", [], [])


main()

# follow-up:
# 1. any binary tree (LC 117) -> per level keep a dummy head + tail for the next level
# 2. BFS version -> queue per level, link each node to the next one popped
