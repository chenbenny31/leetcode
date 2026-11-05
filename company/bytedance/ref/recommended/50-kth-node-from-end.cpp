// ByteDance (hack2hire): return the value of the 5th node from the end of a singly
// linked list (length >= 5) in one pass with O(1) space. Generalized to k; -1 if
// the list has fewer than k nodes.

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

// Two pointers k apart; when lead falls off the end, trail is k-th from the end.
// T: O(n), S: O(1).
int kthFromEnd(ListNode* head, int k) {
    ListNode* lead = head;
    for (int i = 0; i < k; i++) {
        if (lead == nullptr) {
            return -1;
        }
        lead = lead->next;
    }
    ListNode* trail = head;
    while (lead != nullptr) {
        lead = lead->next;
        trail = trail->next;
    }
    return trail->val;
}

void test(const string& label, const vector<int>& vals, int k, int want) {
    // the list; link only after all pushes, so addresses hold
    vector<ListNode> nodes;
    for (int v : vals) {
        nodes.push_back({v, nullptr});
    }
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) {
        nodes[i].next = &nodes[i + 1];
    }
    cout << label << (kthFromEnd(&nodes[0], k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {1, 2, 3, 4, 5, 6, 7}, 5, 3);
    test("k-is-length", {10, 20, 30, 40, 50}, 5, 10);
    test("too-short", {1, 2, 3}, 5, -1);
}

// follow-up: delete k-th from end (LC 19) -> dummy head, lead k + 1 ahead of trail,
//   then trail->next = trail->next->next
// follow-up: middle node (LC 876) -> slow += 1, fast += 2 until fast or fast->next null
// follow-up: rotate right by k (LC 61) -> len, k %= len, cut before k-th from end,
//   old tail->next = head
