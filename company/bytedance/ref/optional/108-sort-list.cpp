// ByteDance / LC 148: sort a singly linked list in ascending order and return the head.
// Follow-up: O(n log n) time with O(1) extra space.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
};

// Splice two sorted lists behind a dummy. T: O(a+b), S: O(1).
ListNode* merge(ListNode* a, ListNode* b) {
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
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

// cut off the first k nodes of head; return the head of the rest.
ListNode* split(ListNode* head, int k) {
    for (int i = 1; head && i < k; i++) { head = head->next; }
    if (!head) { return nullptr; }
    ListNode* rest = head->next;
    head->next = nullptr;
    return rest;
}

// follow-up: bottom-up merge sort, runs of size 1, 2, 4, ... T: O(n log n), S: O(1).
ListNode* sortListBottomUp(ListNode* head) {
    int n = 0;
    for (ListNode* cur = head; cur; cur = cur->next) { n++; }
    ListNode dummy{0, head};
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

vector<int> toVector(ListNode* head) {
    vector<int> vals;
    for (ListNode* cur = head; cur; cur = cur->next) { vals.push_back(cur->val); }
    return vals;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    // two copies of the list; link only after all pushes, so addresses hold
    vector<ListNode> a;
    vector<ListNode> b;
    for (int v : vals) {
        a.push_back({v, nullptr});
        b.push_back({v, nullptr});
    }
    int n = vals.size();
    for (int i = 0; i + 1 < n; i++) {
        a[i].next = &a[i + 1];
        b[i].next = &b[i + 1];
    }
    bool ok = toVector(sortList(n ? &a[0] : nullptr)) == want
              && toVector(sortListBottomUp(n ? &b[0] : nullptr)) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {4, 2, 1, 3}, {1, 2, 3, 4});
    test("negatives", {-1, 5, 3, 4, 0}, {-1, 0, 3, 4, 5});
    test("empty", {}, {});
}

// follow-up: O(1) extra space -> see sortListBottomUp() above; split runs of 1, 2, 4, ...
//   and merge, no recursion
// follow-up: why merge sort for lists -> no random access needed, merge relinks in O(1)
//   extra, stable; quicksort degrades to O(n^2) on bad pivots
// follow-up: insertion sort list (LC 147) -> dummy head, splice each node after the last
//   smaller one, O(n^2)
