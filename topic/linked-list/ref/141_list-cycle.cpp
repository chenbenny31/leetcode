// LC 141 Linked List Cycle: return true if following next pointers from head ever
// loops back to an earlier node.
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

// Floyd: fast moves two steps, slow one; they meet iff there is a cycle.
// T: O(L), S: O(1).
bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { return true; }
    }
    return false;
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
void test(const string& label, const vector<int>& vals, int pos, bool want) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    if (pos >= 0) { pool.back().next = &pool[pos]; }
    cout << label << (hasCycle(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cycle-to-head", {1, 2, 3}, 0, true);
    test("cycle-to-middle", {1, 2, 3, 4}, 2, true);
    test("no-cycle", {1, 2, 3}, -1, false);
    test("empty", {}, -1, false);
}
