// LC 148 Sort List (Medium). Pattern: merge sort on a linked list.
// Sort a singly linked list ascending and return the head.
// Follow-up in the problem: O(n log n) time with O(1) extra space.

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

// splice two sorted lists behind a dummy
ListNode* merge(ListNode* left, ListNode* right) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (left && right) {
        if (left->val <= right->val) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    tail->next = left ? left : right;
    return dummy.next;
}

// Top-down merge sort; slow/fast finds the middle. T: O(n log n), S: O(log n) stack.
ListNode* sortList(ListNode* head) {
    if (!head || !head->next) { return head; }
    // fast starts one ahead so slow stops at the end of the left half
    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* right = slow->next;
    slow->next = nullptr;
    return merge(sortList(head), sortList(right));
}

// cut off the first size nodes of head; return the head of the rest
ListNode* split(ListNode* head, int size) {
    for (int i = 1; head && i < size; i++) { head = head->next; }
    if (!head) { return nullptr; }
    ListNode* rest = head->next;
    head->next = nullptr;
    return rest;
}

// Follow-up: bottom-up, merge runs of size 1, 2, 4, ... T: O(n log n), S: O(1).
ListNode* sortListBottomUp(ListNode* head) {
    int n = 0;
    for (ListNode* cur = head; cur; cur = cur->next) { n++; }
    ListNode dummy(0);
    dummy.next = head;
    for (int size = 1; size < n; size *= 2) {
        // last node of the merged part of this pass
        ListNode* tail = &dummy;
        ListNode* cur = dummy.next;
        while (cur) {
            ListNode* left = cur;
            ListNode* right = split(left, size);
            cur = split(right, size);
            tail->next = merge(left, right);
            while (tail->next) { tail = tail->next; }
        }
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
    vector<ListNode> poolTopDown;
    vector<ListNode> poolBottomUp;
    bool ok = toVec(sortList(build(poolTopDown, vals))) == want
              && toVec(sortListBottomUp(build(poolBottomUp, vals))) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups-odd-length", {3, 1, 2, 1, 3}, {1, 1, 2, 3, 3});
    test("negatives", {0, -3, 5, -1}, {-3, -1, 0, 5});
    test("single", {7}, {7});
    test("empty", {}, {});
}

// follow-up: why merge sort for lists -> no random access needed and merge relinks in
//   O(1) extra space; quicksort partitioning is awkward and O(n^2) on bad pivots
