# LC 142 Linked List Cycle II (Medium). Pattern: Floyd fast/slow pointers.
# Return the node where the cycle begins, or null if the list has no cycle. O(1) space.


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# After slow and fast meet, a walker from head and one from the meeting point reach the
# entry together. T: O(n), S: O(1).
def detect_cycle(head):
    slow = head
    fast = head
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next
        if slow is fast:
            # slow walked a + b, fast 2(a + b): a + b is a multiple of the cycle length,
            # so a steps from the meeting point land on the entry
            cur = head
            while cur is not slow:
                cur = cur.next
                slow = slow.next
            return cur
    return None


# pos = index the last node points back to, -1 for no cycle; want = that node
def test(label, vals, pos):
    # nodes[i] = the i-th node of the list
    nodes = [ListNode(v) for v in vals]
    for i in range(len(nodes) - 1):
        nodes[i].next = nodes[i + 1]
    want = None
    if pos >= 0:
        nodes[-1].next = nodes[pos]
        want = nodes[pos]
    ok = detect_cycle(nodes[0]) is want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("entry-middle", [1, 2, 3, 4, 5], 2)
    test("entry-head", [1, 2, 3], 0)
    test("self-loop", [1, 2, 3], 2)
    test("no-cycle", [1, 2, 3], -1)


main()

# follow-up: cycle length -> after meeting, step slow around until it is back at fast
# follow-up: O(n) space allowed -> set of visited nodes; first repeat = entry
