// LC 21 Merge Two Sorted Lists: merge two sorted lists by splicing their nodes into one
// sorted list and return its head.
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

// Dummy head + tail pointer; take the smaller front node, then append the leftover
// list whole. T: O(m + n), S: O(1).
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* tail = &dummy;   // last node of the merged list so far
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

void test(const string& label, const vector<int>& a, const vector<int>& b,
          const vector<int>& want) {
    vector<ListNode> pa, pb;
    cout << label << (toVec(mergeTwoLists(build(pa, a), build(pb, b))) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 3, 5}, {2, 4}, {1, 2, 3, 4, 5});
    test("duplicates", {1, 1}, {1}, {1, 1, 1});
    test("one-empty", {}, {2, 3}, {2, 3});
    test("both-empty", {}, {}, {});
}
