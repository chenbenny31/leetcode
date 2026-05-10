// find middle + reverse + merge, T: O(n), S: O(1)

#include <cstddef> // nullptr

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) { return; }

        // find middle
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast ->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // reverse second half
        ListNode* prev = nullptr;
        ListNode* curr = slow->next;
        slow->next = nullptr;
        while (curr) {
            ListNode* succ = curr->next;
            curr->next = prev;
            prev = curr;
            curr = succ;
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

// fast = head->next: ensure slow stops at first half ending
// slow->next = nullptr: mandatory cut
// cache three sequentail passes
// termiante at second over first: second half if shorter or equal
