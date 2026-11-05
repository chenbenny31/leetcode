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

// Count nodes, then reverse n / k full groups one by one, each hooked after prevTail.
// T: O(n), S: O(1).
ListNode* reverseKGroup(ListNode* head, int k) {
    int n = 0;
    for (ListNode* cur = head; cur != nullptr; cur = cur->next) {
        n++;
    }
    ListNode dummy{0, head};
    // last node of the part already done
    ListNode* prevTail = &dummy;
    // n / k: a short last group is never touched
    for (int g = 0; g < n / k; g++) {
        ListNode* groupHead = prevTail->next;
        ListNode* prev = nullptr;
        ListNode* cur = groupHead;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        // prev = new group head, groupHead = new group tail, cur = next group's head
        prevTail->next = prev;
        groupHead->next = cur;
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
}

// follow-up: reverse the short tail too -> one more pass of n % k steps after the loop
// follow-up: groups counted from the tail -> advance prevTail n % k nodes first, then
//   same loop, O(n)
// follow-up: recursive -> reverse first k, groupHead->next = reverseKGroup(rest, k),
//   O(n / k) stack
