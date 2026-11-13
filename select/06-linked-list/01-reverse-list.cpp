// LC 206 Reverse Linked List (Easy) — in-place pointer reversal, iterative / recursive
// reverse a singly linked list; return new head
//
// clarify:
// empty / single node -> return head as is
// in place? -> yes, no new nodes
// list len -> long list overflows recursion, use iterative
// iterative or recursive? -> both below

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

// solution 1: iterative; default, O(1) space
// core idea: walk once, flip each next back to prev; prev ends as new head
// T: O(n), S: O(1)
ListNode* reverseList(ListNode* head) {
    // head of the reversed part
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

// solution 2: recursive; only when asked for it
// core idea: reverse the rest, then hang head after its old next (= tail of the rest)
// T: O(n), S: O(n) — recursion stack
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

// follow-up:
// reverse positions left..right (LC 92) -> dummy, walk to node before left, head-insert
// reverse in groups of k (LC 25) -> check k nodes remain, reverse, hook to prev tail
// swap nodes in pairs (LC 24) -> k = 2 case of group reversal
// palindrome list (LC 234) -> mid by slow / fast, reverse 2nd half, compare
// reorder list L0, Ln, L1, Ln-1 (LC 143) -> mid, reverse 2nd half, interleave
