# LC 21 Merge Two Sorted Lists (Easy). Pattern: dummy head + tail pointer.
# Splice the nodes of two sorted lists into one sorted list and return its head.


class ListNode:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


# Take the smaller front node each step, then append the leftover list whole.
# T: O(m + n), S: O(1).
def merge_two_lists(list1, list2):
    dummy = ListNode(0)
    # last node of the merged list so far
    tail = dummy
    while list1 and list2:
        if list1.val <= list2.val:
            tail.next = list1
            list1 = list1.next
        else:
            tail.next = list2
            list2 = list2.next
        tail = tail.next
    if list1:
        tail.next = list1
    else:
        tail.next = list2
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


def test(label, vals1, vals2, want):
    head = merge_two_lists(build_list(vals1), build_list(vals2))
    print(label, "PASS" if to_list(head) == want else "FAIL")


def main():
    test("interleave", [1, 3, 5], [2, 4], [1, 2, 3, 4, 5])
    test("duplicates", [1, 1], [1], [1, 1, 1])
    test("one-empty", [], [2, 3], [2, 3])


main()

# follow-up: merge k sorted lists (LC 23) -> heapq of (val, list index, node) for the k
#   current heads (the index breaks ties, ListNode is not comparable); pop the smallest,
#   push its next, O(N log k)
# follow-up: recursive -> smaller head wins, head.next = merge(head.next, other)
