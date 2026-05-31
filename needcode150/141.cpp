// Floyd's cycle detection. T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) { return true; }
        }
        return false;
    }
};

// fast && fast->next two null checks: prevent fast->next->ext trigger null de-reference
// compare pointer not value: same val won't trigger ending case
// cache fast clear cache miss for slow
// prove: O(steps-to-entry + cycle-len) = O(n)
