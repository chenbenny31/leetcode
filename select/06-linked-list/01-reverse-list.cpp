// LC 206 Reverse Linked List (Easy). Pattern: three-pointer in-place reversal.
// Reverse a singly linked list and return the new head.

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

// Walk once, flipping each next pointer back to prev. T: O(n), S: O(1).
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

// Recursive: reverse the rest, then hang head after its old next. T: O(n), S: O(n) stack.
ListNode* reverseListRec(ListNode* head) {
    if (!head || !head->next) { return head; }
    ListNode* newHead = reverseListRec(head->next);
    // head->next is now the tail of the reversed rest
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

// LC 92 (1-indexed left <= right): dummy before head, walk to the node before left,
// then move each following node to the front of the sublist, right - left times.
// T: O(n), S: O(1).
ListNode* reverseBetween(ListNode* head, int left, int right) {
    // dummy.next = head, so left == 1 needs no special case
    ListNode dummy(0);
    dummy.next = head;
    // node just before the sublist; stays fixed
    ListNode* before = &dummy;
    for (int i = 1; i < left; i++) { before = before->next; }
    // first node of the sublist; ends up as its tail
    ListNode* tail = before->next;
    for (int i = 0; i < right - left; i++) {
        // unlink the node after tail and insert it right after before
        ListNode* moved = tail->next;
        tail->next = moved->next;
        moved->next = before->next;
        before->next = moved;
    }
    return dummy.next;
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
    vector<ListNode> poolIter;
    vector<ListNode> poolRec;
    bool ok = toVec(reverseList(build(poolIter, vals))) == want
              && toVec(reverseListRec(build(poolRec, vals))) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testBetween(const string& label, const vector<int>& vals, int left, int right,
                 const vector<int>& want) {
    vector<ListNode> pool;
    bool ok = toVec(reverseBetween(build(pool, vals), left, right)) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3}, {3, 2, 1});
    test("single", {7}, {7});
    test("empty", {}, {});
    testBetween("between-middle", {1, 2, 3, 4}, 2, 3, {1, 3, 2, 4});
    testBetween("between-whole-list", {1, 2, 3}, 1, 3, {3, 2, 1});
    testBetween("between-same-pos", {1, 2}, 2, 2, {1, 2});
}
