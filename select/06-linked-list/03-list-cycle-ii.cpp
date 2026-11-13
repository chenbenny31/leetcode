// LC 142 Linked List Cycle II (Medium) — Floyd slow / fast pointers
// Return the node where the cycle begins, or null if the list has no cycle.
// Clarify: O(1) extra space required; the list must not be modified; empty list and
//   single node; return the node, not its index.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Core idea: slow moves 1 and fast 2; if they meet, a walker from head and one from the
// meeting point, both moving 1, reach the cycle entry together. T: O(n), S: O(1).
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // slow walked a + b, fast 2(a + b): a + b is a multiple of the cycle length,
            // so a steps from the meeting point land on the entry
            ListNode* cur = head;
            while (cur != slow) {
                cur = cur->next;
                slow = slow->next;
            }
            return cur;
        }
    }
    return nullptr;
}

ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

void test(const string& label, const vector<int>& vals, int pos) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    ListNode* want = nullptr;
    if (pos >= 0) {
        pool.back().next = &pool[pos];
        want = &pool[pos];
    }
    cout << label << (detectCycle(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("entry-middle", {1, 2, 3, 4, 5}, 2);
    test("entry-head", {1, 2, 3}, 0);
    test("self-loop", {1, 2, 3}, 2);
    test("no-cycle", {1, 2, 3}, -1);
    return 0;
}

// follow-up (ideas only):
// - only say whether a cycle exists (LC 141) -> return true at the first meeting
// - cycle length -> after the meeting, step slow around until it is back at fast
// - O(n) space allowed -> unordered_set of visited nodes; the first repeat is the entry
// - find the duplicate, n + 1 values in [1, n] (LC 287) -> index i is a node and nums[i]
//   its next; index 0 lies outside the cycle and the duplicate value is the cycle entry
// - happy number (LC 202) -> slow / fast on the digit-square-sum sequence; a meeting at a
//   value other than 1 means false
// - middle of the list (LC 876) -> slow is at the middle when fast reaches the end
// - remove the n-th node from the end (LC 19) -> two pointers n apart, starting from a
//   dummy head; when the front one hits the end the back one is just before the target
// - palindrome list (LC 234) / reorder list (LC 143) -> find the middle with slow /
//   fast, reverse the second half, then compare / interleave the two halves
