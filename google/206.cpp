// iterative, T: O(n), S: O(1)

#include <cstddef> // nullptr

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

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

#include <cstddef> // nullptr

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) { return head; }

        ListNode* newHead = reverseList(head->next);
        head->next->next = head; // head becomes new tail
        head->next = nullptr;
        return newHead;
    }
};

// prev = nullptr initailization: original head will becomes new tail
// head->next->next = head: safe because head->next guarantees non-null
// recursive stack depth O(n): n = 1e5 ~ 1.6MB stack usage
// cache recursive has double traversal cost: write on the way down and read on the way up
