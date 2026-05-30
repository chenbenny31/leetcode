// iterative, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
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

// recursive, T: O(n), S: O(n) stack frames

#include <cstddef>

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) { return head; }
        ListNode* newHead = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};

// resursive stack depth O(n), one per node
// cache recursive stack writes on way down, read on way up, double traversal cost
