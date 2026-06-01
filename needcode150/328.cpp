// two-pointers, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (!head || !head->next) { return head; }

        ListNode* odd = head;
        ListNode* even = head->next;
        ListNode* even_head = even;

        while (even && even->next) {
            odd->next = even->next;
            odd = odd->next;
            even->next = odd->next;
            even = even->next;
        }
        odd->next = even_head;
        return head;
    }
};

// even_head saved upfront 
// index-based not value-based: positions of odd/even
