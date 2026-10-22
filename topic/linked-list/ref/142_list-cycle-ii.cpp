// LC 142 Linked List Cycle II: return the node where the cycle begins, or null if the
// list has no cycle.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Floyd in two phases. Let a = head to entry, b = entry to meeting point, c = cycle
// length. At the meeting slow walked a + b, fast 2(a + b), so a + b is a multiple of c
// and a = k * c - b: a walker from head and one from the meeting point, both one step
// at a time, arrive at the entry together. T: O(L), S: O(1).
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow != fast) { continue; }
        ListNode* cur = head;
        while (cur != slow) {
            cur = cur->next;
            slow = slow->next;
        }
        return cur;
    }
    return nullptr;
}

ListNode* build(vector<ListNode>& pool, const vector<int>& vals,
                ListNode* rest = nullptr) {
    int n = vals.size();
    pool.assign(n, ListNode(0));
    for (int i = 0; i < n; i++) {
        pool[i].val = vals[i];
        pool[i].next = i + 1 < n ? &pool[i + 1] : rest;
    }
    return n > 0 ? &pool[0] : rest;
}

vector<int> toVec(ListNode* head) {
    vector<int> out;
    for (ListNode* cur = head; cur; cur = cur->next) {
        out.push_back(cur->val);
    }
    return out;
}

// pos = index the last node points back to, or -1 for no cycle
void test(const string& label, const vector<int>& vals, int pos) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    if (pos >= 0) { pool.back().next = &pool[pos]; }
    ListNode* want = pos >= 0 ? &pool[pos] : nullptr;
    cout << label << (detectCycle(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("entry-middle", {1, 2, 3, 4, 5}, 2);
    test("entry-head", {1, 2}, 0);
    test("self-loop", {1, 2, 3}, 2);
    test("no-cycle", {1, 2, 3}, -1);
}
