# LC 206 Reverse Linked List (Easy). Pattern: three-pointer in-place reversal.
# Reverse a singly linked list and return the new head.


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# Walk once, flipping each next pointer back to prev. T: O(n), S: O(1).
def reverse_list(head):
    # head of the already reversed part
    prev = None
    cur = head
    while cur:
        # save before overwriting cur.next
        nxt = cur.next
        cur.next = prev
        prev = cur
        cur = nxt
    return prev


# Recursive: reverse the rest, then hang head after its old next. T: O(n), S: O(n) stack.
def reverse_list_rec(head):
    if head is None or head.next is None:
        return head
    new_head = reverse_list_rec(head.next)
    # head.next is now the tail of the reversed rest
    head.next.next = head
    head.next = None
    return new_head


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
    ok = to_list(reverse_list(build_list(vals))) == want
    ok = ok and to_list(reverse_list_rec(build_list(vals))) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("three", [1, 2, 3], [3, 2, 1])
    test("single", [7], [7])
    test("empty", [], [])


main()

# follow-up: reverse only positions left..right (LC 92) -> walk to the node before left,
#   then move each following node to the front of the sublist, right - left times
