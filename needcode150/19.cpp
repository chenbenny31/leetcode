// two-pointers, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        for (int i = 0; i <= n; i++) { fast = fast->next; }

        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        slow->next = slow->next->next;
        return dummy.next;
    }
};

// adv fast by n+1 not n: slow need to stop at node before target
// dummy head handles natural remove of head node, e.g. len = 1, n = 1
// while (fast) not while (fast->next): fast->next stops when fast at last, while (fast) stops when fast is null
