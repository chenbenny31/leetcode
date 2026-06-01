// find tail + reconnect, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) { return head; }

        // find length and tail
        int n = 1;
        ListNode* tail = head;
        while (tail->next) {
            tail = tail->next;
            n++;
        }

        k %= n;
        if (k == 0) { return head; }

        // find new tail: n - k - 1 steps from head
        ListNode* new_tail = head;
        for (int i = 0; i < n - k - 1; i++) { new_tail = new_tail->next; }

        ListNode* new_head = new_tail->next;
        new_tail->next = nullptr;
        tail->next = head; 
        return new_head;
    }
};

// ring-cut, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) { return head; }

        int n = 1;
        ListNode* tail = head;
        while (tail->next) {
            tail = tail->next;
            n++;
        }

        k %= n;
        if (k == 0) { return head; }

        tail->next = head; // form ring
        ListNode* new_tail = tail; // adv n - k steps from tail to find new tail
        for (int i = 0; i < n - k; i++) { new_tail = new_tail->next; }

        ListNode* new_head = new_tail->next;
        new_tail->next = nullptr;
        return new_head;
    }
};

// k %= n mandatory: otherwise n - k - 1 goes negative
// n - k - 1 not n - k: steps land on one before new head
// ring vs two-pass: ring is theoreticall better, one traversal of n-k steps
