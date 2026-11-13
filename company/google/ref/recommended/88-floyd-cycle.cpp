// 88 · Linked List Cycle II (Floyd) — LC 142, 287
// Mother: select/06-linked-list/03-list-cycle-ii.cpp
// return the node where the cycle begins, null if no cycle; O(1) extra space
// reported: find the duplicate
//
// clarify:
// return what -> the entry node, not its idx or value
// no cycle -> nullptr
// empty list -> nullptr
// extra space -> O(1), no visited set

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

// core idea: slow +1, fast +2; at the meet slow walked a + b, fast 2(a + b)
// a + b = multiple of cycle len -> walkers from head and meet point reach entry together
// T: O(n), S: O(1)
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
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

// pos = idx the last node points back to, -1 = no cycle; want = that node
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

// follow-up:
// find the duplicate (LC 287, Google) -> idx i = node, nums[i] = its next; same Floyd
// LC 287 start -> idx 0 (no value is 0, so it is outside the cycle); entry = duplicate
// cycle len -> after meeting, step slow around until it is back at fast
// O(n) space allowed -> unordered_set of visited nodes; first repeat = entry
