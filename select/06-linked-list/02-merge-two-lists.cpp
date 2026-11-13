// LC 21 Merge Two Sorted Lists (Easy) — dummy head + tail pointer
// Splice the nodes of two sorted lists into one sorted list and return its head.
// Clarify: either list may be empty; reuse the nodes, no new ones; duplicates are kept;
//   both lists ascending.

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

// Core idea: hang the smaller front node after tail each step, then append the leftover
// list whole; the dummy removes the empty-result special case. T: O(m + n), S: O(1).
ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
    ListNode dummy(0);
    // last node of the merged list so far
    ListNode* tail = &dummy;
    while (list1 && list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    tail->next = list1 ? list1 : list2;
    return dummy.next;
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

void test(const string& label, const vector<int>& vals1, const vector<int>& vals2,
          const vector<int>& want) {
    vector<ListNode> pool1;
    vector<ListNode> pool2;
    ListNode* head = mergeTwoLists(build(pool1, vals1), build(pool2, vals2));
    cout << label << (toVec(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 3, 5}, {2, 4}, {1, 2, 3, 4, 5});
    test("duplicates", {1, 1}, {1}, {1, 1, 1});
    test("one-empty", {}, {2, 3}, {2, 3});
    return 0;
}

// follow-up (ideas only):
// - recursive version -> the smaller head wins and head->next = merge(head->next, other);
//   O(m + n) stack
// - merge k sorted lists (LC 23) -> min-heap of the k current heads; pop the smallest,
//   push its next, O(N log k); or merge pairwise by divide and conquer
// - add two numbers, digits least significant first (LC 2) -> same dummy + tail walk with
//   a carry, one new node per column, loop while a list or the carry remains
// - sort list (LC 148) -> split at the middle by slow / fast, sort both halves, then
//   merge them with this function
// - merge sorted arrays in place (LC 88) -> fill from the back with the larger tail value
