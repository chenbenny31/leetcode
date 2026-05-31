// Floyd's cycle detection, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* detectCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        // find meeting point
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) { break; }
        }

        if (!fast || !fast->next) { return nullptr; }

        // find cycle entry
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        return slow;
    }
};

// find cycle entry math: fast-low = (dist-to-entry + x) = k * (cycle-len), dist-to-entry = k*cycle-len - x, reset slow step dist-to-entry
// if (!fast || !fast->next) verify: must check termination case
// case cycle start at head: after break slow meets fast immediately
