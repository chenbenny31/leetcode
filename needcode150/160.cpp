// two-pointers, T: O(m+n), S: O(1)

#include <cstddef> // nullptr

class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        ListNode* a = headA;
        ListNode* b = headB;

        while (a != b) {
            a = a ? a->next : headB;
            b = b ? b->next : headA;
        }
        return a;
    }
};

// correctness on meeting at intersection: a travels la + lb - c while b traverse lb + la - c, arrive intersection simultaneously
// no intersection case: both reach nullptr, a == b == nullptr
// a != b addr comp not val
