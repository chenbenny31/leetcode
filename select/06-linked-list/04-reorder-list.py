# LC 143 Reorder List (Medium). Pattern: middle, reverse second half, merge.
# Reorder L0 -> L1 -> ... -> Ln into L0 -> Ln -> L1 -> Ln-1 -> ... in place by relinking
# nodes (values may not change).


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# Find the middle, reverse the second half, then merge the halves alternately.
# T: O(n), S: O(1).
def reorder_list(head):
    if head is None:
        return
    slow = head
    fast = head
    # slow stops at the end of the first half (the first half keeps the extra node)
    while fast.next and fast.next.next:
        slow = slow.next
        fast = fast.next.next
    prev = None
    cur = slow.next
    # cut the halves apart before reversing the second one
    slow.next = None
    while cur:
        nxt = cur.next
        cur.next = prev
        prev = cur
        cur = nxt
    first = head
    second = prev
    # the second half is never longer than the first
    while second:
        first_next = first.next
        second_next = second.next
        first.next = second
        second.next = first_next
        first = first_next
        second = second_next


# links new nodes holding vals in order; returns the head
def build_list(vals):
    dummy = ListNode(0)
    tail = dummy
    for v in vals:
        tail.next = ListNode(v)
        tail = tail.next
    return dummy.next


def to_list(head):
    out = []
    cur = head
    while cur:
        out.append(cur.val)
        cur = cur.next
    return out


def test(label, vals, want):
    head = build_list(vals)
    reorder_list(head)
    print(label, "PASS" if to_list(head) == want else "FAIL")


def main():
    test("single", [1], [1])
    test("two-nodes", [1, 2], [1, 2])
    test("odd", [1, 2, 3], [1, 3, 2])
    test("even", [1, 2, 3, 4, 5, 6], [1, 6, 2, 5, 3, 4])


main()

# follow-up: simpler O(n) space -> put the nodes in a list, link i and j inward from
#   both ends, then set the last node's next to None
# follow-up: palindrome list (LC 234) -> same middle + reverse, then compare the halves
