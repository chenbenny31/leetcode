// digit-by-digit with carry, T: O(max(m, n)), S: O(max(m, n))

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        int carry = 0;

        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }

            carry = sum / 10;
            curr->next = new ListNode(sum % 10);
            curr = curr->next;
        }
        return dummy.next;
    }
};

// while (l1 || l2 || carry) three way term: handles unequal len and trailling carry
// why not proc backwards: no knowing carry
