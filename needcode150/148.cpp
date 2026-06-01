// top-down merge sort, T: O(nlogn), S: O(logn) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) { return head; }

        ListNode* mid = get_mid(head);
        ListNode* right = mid->next;
        mid->next = nullptr;

        ListNode* l = sortList(head);
        ListNode* r = sortList(right);
        return merge(l, r);
    }

private:
    ListNode* get_mid(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        curr->next = l1 ? l1 : l2;
        return dummy.next;
    }
};

// bottom-up merge sort, T: O(n * log(n)), S: O(1)

#include <cstddef>
#include <utility> // std::pair

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        int n = 0;
        ListNode* curr = head;
        while (curr) {
            curr = curr->next;
            n++;
        }
        
        ListNode dummy(0);
        dummy.next = head;

        for (int size = 1; size < n; size *= 2) {
            ListNode* tail = &dummy;
            curr = dummy.next;

            while (curr) {
                ListNode* left = curr;
                ListNode* right = split(left, size);
                curr = split(right, size);
                auto [merged_head, merged_tail] = merge(left, right);
                tail->next = merged_head;
                tail = merged_tail;
            }
        }
        return dummy.next;
    }

private:
    ListNode* split(ListNode* head, int size) {
        while (head && size > 1) {
            head = head->next;
            size--;
        }
        if (!head) { return nullptr; }
        ListNode* rest = head->next;
        head->next = nullptr;
        return rest;
    }

    std::pair<ListNode*, ListNode*> merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        curr->next = l1 ? l1 : l2;
        while (curr->next) { curr = curr->next; } // adv to tail
        return {dummy.next, curr};
    }
};

// top-down O(logn) stack: one frame per recursion level, safe for n=10^5 with ~17 frames
// bottom-up O(1) space, no recursion, zero stack overhead
// count n upgront in bottom-up
// two-down vs bottom-up: bottom-up is space opt of two-down
