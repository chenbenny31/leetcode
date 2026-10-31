// ByteDance (FastPrep #51, full-time phone) / LC 25: reverse the nodes of a singly linked
// list k at a time and return the new head. A last group with fewer than k nodes stays
// as it is. Only the next pointers may change, never the node values.
// 1 <= k <= number of nodes <= 5000.
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

// Go group by group. prevTail is the last node of the part already done (a dummy at the
// start). First check that k nodes remain, then reverse exactly those k nodes and hook
// the reversed group between prevTail and the rest of the list. T: O(n), S: O(1).
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy{0, head};
    ListNode* prevTail = &dummy;
    while (true) {
        // walk k steps to the last node of the next group; a short group stops the loop
        ListNode* kth = prevTail;
        for (int i = 0; i < k && kth != nullptr; i++) {
            kth = kth->next;
        }
        if (kth == nullptr) {
            break;
        }
        ListNode* groupHead = prevTail->next;
        ListNode* after = kth->next;

        // reverse groupHead..kth; starting prev at `after` links the group's new tail
        // to the rest of the list for free
        ListNode* prev = after;
        ListNode* cur = groupHead;
        while (cur != after) {
            ListNode* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }

        // kth is now the group's head and groupHead its tail
        prevTail->next = kth;
        prevTail = groupHead;
    }
    return dummy.next;
}

void test(const string& label, const vector<int>& vals, int k, const vector<int>& want) {
    vector<ListNode> nodes(vals.size());
    for (int i = 0; i < (int)vals.size(); i++) {
        nodes[i].val = vals[i];
        nodes[i].next = nullptr;
        if (i + 1 < (int)vals.size()) {
            nodes[i].next = &nodes[i + 1];
        }
    }
    vector<int> got;
    for (ListNode* cur = reverseKGroup(&nodes[0], k); cur != nullptr; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k-one", {1, 2, 3}, 1, {1, 2, 3});
    test("whole-list", {1, 2, 3}, 3, {3, 2, 1});
    test("even-groups", {1, 2, 3, 4, 5, 6}, 3, {3, 2, 1, 6, 5, 4});
    test("short-tail", {1, 2, 3, 4, 5, 6}, 4, {4, 3, 2, 1, 5, 6});
    test("single", {7}, 1, {7});
}
