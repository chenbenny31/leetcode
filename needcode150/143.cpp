// find middle + reverse + merge, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) { return; }

        // find middle
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // reverse second half
        ListNode* prev = nullptr;
        ListNode* curr = slow->next;
        slow->next = nullptr; // cut list at middle
        while (curr) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        // merge two halves
        ListNode* first = head;
        ListNode* second = prev;
        while (second) {
            ListNode* succ1 = first->next;
            ListNode* succ2 = second->next;
            first->next = second;
            second->next = succ1;
            first = succ1;
            second = succ2;
        }
    }
};

// fast = head->next find middle: for even len, make slow lands on end of first half
