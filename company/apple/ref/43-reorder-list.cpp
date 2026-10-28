// LC 143: reorder L0 -> L1 -> ... -> Ln into L0 -> Ln -> L1 -> Ln-1 -> ... in place.
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

// straightforward: index the nodes, then link from both ends inward.
// T: O(n), S: O(n).
void reorderListVec(ListNode* head) {
    vector<ListNode*> nodes;   // nodes[i] = i-th node of the list
    for (ListNode* cur = head; cur; cur = cur->next) { nodes.push_back(cur); }
    int i = 0, j = (int)nodes.size() - 1;
    while (i < j) {
        nodes[i]->next = nodes[j];
        i++;
        if (i == j) { break; }
        nodes[j]->next = nodes[i];
        j--;
    }
    // i is the new tail
    if (!nodes.empty()) { nodes[i]->next = nullptr; }
}

// improved: find the middle, reverse the second half, merge alternately.
// T: O(n), S: O(1).
void reorderList(ListNode* head) {
    if (!head) { return; }
    ListNode* slow = head;
    ListNode* fast = head;
    // slow stops at the end of the first half (first half keeps the extra node)
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // head of the reversed second half
    ListNode* prev = nullptr;
    ListNode* cur = slow->next;
    // cut the two halves apart
    slow->next = nullptr;
    while (cur) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    ListNode* first = head;
    ListNode* second = prev;
    // second half is never longer than the first
    while (second) {
        ListNode* n1 = first->next;
        ListNode* n2 = second->next;
        first->next = second;
        second->next = n1;
        first = n1;
        second = n2;
    }
}

// build a fresh list from vals, reorder it, return the values in list order
vector<int> run(bool useVec, const vector<int>& vals) {
    int n = vals.size();
    vector<ListNode> nodes(n);
    for (int i = 0; i < n; i++) {
        nodes[i] = {vals[i], i + 1 < n ? &nodes[i + 1] : nullptr};
    }
    ListNode* head = n == 0 ? nullptr : &nodes[0];
    if (useVec) {
        reorderListVec(head);
    } else {
        reorderList(head);
    }
    vector<int> result;
    for (ListNode* cur = head; cur; cur = cur->next) { result.push_back(cur->val); }
    return result;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    bool ok = run(true, vals) == want && run(false, vals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even", {1, 2, 3, 4}, {1, 4, 2, 3});
    test("odd", {1, 2, 3, 4, 5}, {1, 5, 2, 4, 3});
    test("one", {1}, {1});
    test("empty", {}, {});
}
