# LC 148 Sort List (Medium). Pattern: merge sort on a linked list.
# Sort a singly linked list ascending and return the head.
# Follow-up in the problem: O(n log n) time with O(1) extra space.


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# splice two sorted lists behind a dummy
def merge(left, right):
    dummy = ListNode(0)
    tail = dummy
    while left and right:
        if left.val <= right.val:
            tail.next = left
            left = left.next
        else:
            tail.next = right
            right = right.next
        tail = tail.next
    if left:
        tail.next = left
    else:
        tail.next = right
    return dummy.next


# Top-down merge sort; slow/fast finds the middle. T: O(n log n), S: O(log n) stack.
def sort_list(head):
    if head is None or head.next is None:
        return head
    # fast starts one ahead so slow stops at the end of the left half
    slow = head
    fast = head.next
    while fast and fast.next:
        slow = slow.next
        fast = fast.next.next
    right = slow.next
    slow.next = None
    return merge(sort_list(head), sort_list(right))


# cut off the first size nodes of head; return the head of the rest
def split(head, size):
    i = 1
    while head and i < size:
        head = head.next
        i += 1
    if head is None:
        return None
    rest = head.next
    head.next = None
    return rest


# Follow-up: bottom-up, merge runs of size 1, 2, 4, ... T: O(n log n), S: O(1).
def sort_list_bottom_up(head):
    n = 0
    cur = head
    while cur:
        n += 1
        cur = cur.next
    dummy = ListNode(0)
    dummy.next = head
    size = 1
    while size < n:
        # last node of the merged part of this pass
        tail = dummy
        cur = dummy.next
        while cur:
            left = cur
            right = split(left, size)
            cur = split(right, size)
            tail.next = merge(left, right)
            while tail.next:
                tail = tail.next
        size *= 2
    return dummy.next


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
    ok = to_list(sort_list(build_list(vals))) == want
    ok = ok and to_list(sort_list_bottom_up(build_list(vals))) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("dups-odd-length", [3, 1, 2, 1, 3], [1, 1, 2, 3, 3])
    test("negatives", [0, -3, 5, -1], [-3, -1, 0, 5])
    test("single", [7], [7])
    test("empty", [], [])


main()

# follow-up: why merge sort for lists -> no random access needed and merge relinks in
#   O(1) extra space; quicksort partitioning is awkward and O(n^2) on bad pivots
