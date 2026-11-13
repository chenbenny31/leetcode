// 89 · Merge Two Sorted Lists — LC 21, 2
// Mother: select/06-linked-list/02-merge-two-lists.cpp
// splice the nodes of two sorted lists into one sorted list, return its head
// reported: add two numbers
//
// clarify:
// splice existing nodes or build new ones -> splice
// either list empty -> return the other
// ties -> take from list1 first (stable)

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

// core idea: take the smaller front node each step, then append the leftover list whole
// T: O(m + n), S: O(1)
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
}

// follow-up:
// add two numbers (LC 2, Google) -> digits least significant first: same dummy + tail
// LC 2 step -> new node sum % 10, carry sum / 10; loop while a list or a carry is left
// merge k sorted lists (LC 23) -> min-heap of k heads; pop min, push its next, O(N log k)
// recursive -> smaller head wins, its next = merge of the rest
