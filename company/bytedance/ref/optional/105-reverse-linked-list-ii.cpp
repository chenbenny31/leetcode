// ByteDance / LC 92: reverse the nodes of a singly linked list from position left to
// position right (1-indexed, left <= right) and return the head.
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

// Dummy head; move each node after start to the front of the section. T: O(n), S: O(1).
ListNode* reverseBetween(ListNode* head, int left, int right) {
    // dummy lets left = 1 work like any other position
    ListNode dummy{0, head};
    ListNode* before = &dummy;
    for (int i = 1; i < left; i++) { before = before->next; }
    // start stays put while nodes behind it are moved to the front, one per step
    ListNode* start = before->next;
    for (int i = 0; i < right - left; i++) {
        ListNode* moved = start->next;
        start->next = moved->next;
        moved->next = before->next;
        before->next = moved;
    }
    return dummy.next;
}

void test(const string& label, const vector<int>& vals, int left, int right,
          const vector<int>& want) {
    // the list; link only after all pushes, so addresses hold
    vector<ListNode> nodes;
    for (int v : vals) { nodes.push_back({v, nullptr}); }
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    vector<int> got;
    for (ListNode* cur = reverseBetween(&nodes[0], left, right); cur; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {1, 2, 3, 4, 5}, 2, 4, {1, 4, 3, 2, 5});
    test("single", {5}, 1, 1, {5});
    test("whole-list", {3, 5}, 1, 2, {5, 3});
}

// follow-up: reverse whole list (LC 206) -> prev/cur/nxt loop, O(n) time O(1) space
// follow-up: reverse in groups of k (LC 25) -> check k nodes exist, run the same front-
//   insert loop k - 1 times, before = group's old first node; leftover < k stays
// follow-up: swap pairs (LC 24) -> LC 25 with k = 2
