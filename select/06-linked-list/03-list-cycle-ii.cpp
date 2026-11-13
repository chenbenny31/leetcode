// LC 142 Linked List Cycle II (Medium) — Floyd slow / fast pointers
// return node where the cycle begins, or null if no cycle
//
// clarify:
// extra space -> O(1) required
// modify the list? -> no
// empty / single node -> null unless self-loop
// return -> the node, not its idx

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

// core idea: slow +1, fast +2; if they meet, walk from head and from meeting point at +1
// the two walkers meet at the cycle entry
// T: O(n), S: O(1)
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // slow walked a + b, fast 2(a + b) -> a + b = multiple of cycle len
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

// follow-up:
// only detect a cycle (LC 141) -> return true at first meeting
// cycle len -> after meeting, step slow around until back at fast
// O(n) space ok -> unordered_set of visited nodes; first repeat = entry
// find the duplicate (LC 287) -> idx i is a node, nums[i] its next; dup = cycle entry
// happy number (LC 202) -> slow / fast on digit-square-sum; meet at != 1 -> false
// middle of list (LC 876) -> slow at mid when fast hits end
// remove n-th from end (LC 19) -> two pointers n apart, start from a dummy
// palindrome (LC 234) / reorder (LC 143) -> mid, reverse 2nd half, compare / interleave
