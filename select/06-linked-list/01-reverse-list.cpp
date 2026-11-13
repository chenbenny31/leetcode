// LC 206 Reverse Linked List (Easy) — in-place pointer reversal, iterative / recursive
// Reverse a singly linked list and return the new head.
// Clarify: empty list and single node; in place, no new nodes; list length (recursion
//   depth); iterative or recursive wanted.

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

// Core idea: walk once, flipping each next pointer back to prev; prev ends as the new
// head. T: O(n), S: O(1).
ListNode* reverseList(ListNode* head) {
    // head of the already reversed part
    ListNode* prev = nullptr;
    ListNode* cur = head;
    while (cur) {
        // save before overwriting cur->next
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
}

// Core idea: reverse the rest first, then hang head after its old next, which is now the
// tail of the reversed rest. T: O(n), S: O(n) stack.
ListNode* reverseListRec(ListNode* head) {
    if (!head || !head->next) { return head; }
    ListNode* newHead = reverseListRec(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

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
    vector<ListNode> poolIter;
    vector<ListNode> poolRec;
    bool ok = toVec(reverseList(build(poolIter, vals))) == want
              && toVec(reverseListRec(build(poolRec, vals))) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3}, {3, 2, 1});
    test("single", {7}, {7});
    test("empty", {}, {});
    return 0;
}

// follow-up (ideas only):
// - reverse positions left..right (LC 92) -> dummy before head, walk to the node before
//   left, then right - left times move the node after the sublist's tail to its front
// - reverse in groups of k (LC 25) -> check that k nodes remain, reverse that segment,
//   hook it to the previous group's tail; a short last group stays as it is
// - swap nodes in pairs (LC 24) -> the k = 2 case of the group reversal
// - palindrome list (LC 234) -> middle by slow / fast, reverse the second half, compare
//   the two halves node by node
// - reorder list L0, Ln, L1, Ln-1 (LC 143) -> middle, reverse the second half, interleave
