// one pass in-place, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;

        for (int i = 1; i < left; i++) { prev = prev->next; }

        ListNode* curr = prev->next;
        for (int i = 0; i < right - left; i++) {
            ListNode* succ = curr->next;
            curr->next = succ->next;
            succ->next = prev->next;
            prev->next = succ;
        }
        return dummy.next;
    }
};

// front insertion reverse: each iter reverse succ and curr
