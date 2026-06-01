// iterative in-place, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* group_prev = &dummy;

        while (true) {
            ListNode* kth = get_kth(group_prev, k);
            if (!kth) { break; }

            ListNode* group_next = kth->next;
            ListNode* curr = group_prev->next;

            // front insertion: reverse k nodes
            for (int i = 0; i < k - 1; i++) {
                ListNode* succ = curr->next;
                curr->next = succ->next;
                succ->next = group_prev->next;
                group_prev->next = succ;
            }
            curr->next = group_next; // curr is now tail of reversed group
            group_prev = curr;
        }
        return dummy.next;
    }

private:
    ListNode* get_kth(ListNode* node, int k) {
        while (node && k > 0) {
            node = node->next;
            k--;
        }
        return node;
    }
};

// recursive, T: O(n), S: O(n / k) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        // check k node exist
        ListNode* curr = head;
        int count = 0;
        while (curr && count < k) {
            curr = curr->next;
            count++;
        }
        if (count < k) { return head; } // size fewer than k

        // reverse k nodes
        ListNode* prev = nullptr;
        curr = head;
        for (int i = 0; i < k; i++) {
            ListNode* succ = curr->next;
            curr->next = prev;
            prev = curr;
            curr = succ;
        }
        head->next = reverseKGroup(curr, k); // head is now tail, connected to remainder
        return prev;
    }
};
