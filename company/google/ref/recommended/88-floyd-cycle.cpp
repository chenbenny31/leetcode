// 88 · Linked List Cycle II (Floyd) — LC 142, 287
// Mother problem: select/06-linked-list/03-list-cycle-ii.cpp
//
// LC 142 (Medium), Floyd fast / slow pointers: return the node where the cycle begins,
// or null if the list has no cycle. O(1) extra space.
// Google reports: find the duplicate
//
// Clarify
// - return the entry node, not its index or value; no cycle or empty list → nullptr
//
// Core idea
// - slow +1, fast +2; if they meet, a walker from head and one from the meeting point
//   reach the entry together.
// - Why: slow walked a + b, fast 2(a + b), so a + b is a multiple of the cycle length.
// - T O(n), S O(1).
//
// Tests: entry in the middle, entry at head, self-loop, no cycle
//
// Follow-ups
// - find the duplicate (LC 287) → index i is a node and nums[i] its next; start at
//   index 0 (no value is 0, so it is outside the cycle), same Floyd, entry = duplicate
// - cycle length → after meeting, step slow around until it is back at fast
// - O(n) space allowed → unordered_set of visited nodes; first repeat = entry
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// After slow and fast meet, a walker from head and one from the meeting point reach the
// entry together. T: O(n), S: O(1).
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

// pool owns the nodes; links them in order and returns the head
ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

// pos = index the last node points back to, -1 for no cycle; want = that node
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
}
