// LC 21 Merge Two Sorted Lists (Easy). Pattern: dummy head + tail pointer.
// Splice the nodes of two sorted lists into one sorted list and return its head.

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

// Take the smaller front node each step, then append the leftover list whole.
// T: O(m + n), S: O(1).
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

// LC 2: digits stored least significant first; add column by column with a carry.
// T: O(m + n), S: O(1) extra (the result list is new nodes).
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    // carry into the current column, 0 or 1
    int carry = 0;
    while (l1 || l2 || carry > 0) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        carry = sum / 10;
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
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

void test(const string& label, const vector<int>& vals1, const vector<int>& vals2,
          const vector<int>& want) {
    vector<ListNode> pool1;
    vector<ListNode> pool2;
    ListNode* head = mergeTwoLists(build(pool1, vals1), build(pool2, vals2));
    cout << label << (toVec(head) == want ? " PASS\n" : " FAIL\n");
}

// deletes a list whose nodes came from new
void freeList(ListNode* head) {
    while (head) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

void testAdd(const string& label, const vector<int>& digits1,
             const vector<int>& digits2, const vector<int>& want) {
    vector<ListNode> pool1;
    vector<ListNode> pool2;
    ListNode* sum = addTwoNumbers(build(pool1, digits1), build(pool2, digits2));
    bool ok = toVec(sum) == want;
    freeList(sum);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 3, 5}, {2, 4}, {1, 2, 3, 4, 5});
    test("duplicates", {1, 1}, {1}, {1, 1, 1});
    test("one-empty", {}, {2, 3}, {2, 3});
    testAdd("add-no-carry", {2, 4}, {5, 3}, {7, 7});
    testAdd("add-carry-out", {9, 9}, {1}, {0, 0, 1});
    testAdd("add-zeros", {0}, {0}, {0});
}

// follow-up: merge k sorted lists (LC 23) -> min-heap of the k current heads; pop the
//   smallest, push its next, O(N log k)
// follow-up: recursive -> smaller head wins, head->next = merge(head->next, other)
