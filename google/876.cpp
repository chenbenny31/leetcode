// fast-slow pointers, T: O(n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
};

// fast = head initalization: for even-length list, slow lands on the second middle node, first = head->next return the first middle
// cache fast advance two nodes per step: slow hitting nodes that fast already touches, L1 warm
// one node
