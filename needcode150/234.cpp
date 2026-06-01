// find middle + reverse + compare, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (!head || !head->next) { return true; }

        // find middle
        ListNode* slow = head;
        ListNode* fast = head->next; // slow stops before middle
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // reverse second half
        ListNode* half = reverse(slow->next);
        slow->next = nullptr;

        // compare
        ListNode* first = head;
        ListNode* second = half;
        bool result = true;
        while (second) {
            if (first->val != second->val) { result = false; break; }
            first = first->next;
            second = second->next;
        }

        // restore
        slow->next = reverse(second);
        return result;
    }

private:
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* succ = curr->next;
            curr->next = prev;
            prev = curr;
            curr = succ;
        }
        return prev;
    }
};

// fast = head->next bias: make slow lands before second half
// compare on second not first: second is shorter or equal
// restore after comparison
