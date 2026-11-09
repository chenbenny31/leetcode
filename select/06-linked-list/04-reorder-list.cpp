// LC 143 Reorder List (Medium). Pattern: middle, reverse second half, merge.
// Reorder L0 -> L1 -> ... -> Ln into L0 -> Ln -> L1 -> Ln-1 -> ... in place by relinking
// nodes (values may not change).

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

// Find the middle, reverse the second half, then merge the halves alternately.
// T: O(n), S: O(1).
void reorderList(ListNode* head) {
    if (!head) { return; }
    ListNode* slow = head;
    ListNode* fast = head;
    // slow stops at the end of the first half (the first half keeps the extra node)
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* prev = nullptr;
    ListNode* cur = slow->next;
    // cut the halves apart before reversing the second one
    slow->next = nullptr;
    while (cur) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    ListNode* first = head;
    ListNode* second = prev;
    // the second half is never longer than the first
    while (second) {
        ListNode* firstNext = first->next;
        ListNode* secondNext = second->next;
        first->next = second;
        second->next = firstNext;
        first = firstNext;
        second = secondNext;
    }
}

// pool owns the nodes; links them in order and returns the head
ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

vector<int> toVec(ListNode* head) {
    vector<int> out;
    for (ListNode* cur = head; cur; cur = cur->next) { out.push_back(cur->val); }
    return out;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    reorderList(head);
    cout << label << (toVec(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {1}, {1});
    test("two-nodes", {1, 2}, {1, 2});
    test("odd", {1, 2, 3}, {1, 3, 2});
    test("even", {1, 2, 3, 4, 5, 6}, {1, 6, 2, 5, 3, 4});
}

// follow-up: simpler O(n) space -> push nodes into a vector, link i and j inward from
//   both ends, then set the last node's next to null
// follow-up: palindrome list (LC 234) -> same middle + reverse, then compare the halves
