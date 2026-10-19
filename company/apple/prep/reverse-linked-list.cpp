// reverse a singly linked-list, return the new head

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

// iterative, flip each next pointer when walking, T: O(n), S: O(1)
ListNode* reverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) { return head; }
    ListNode* pred = nullptr;
    ListNode* cur = head;

    while (cur) {
        ListNode* succ = cur->next; // save succ before overwriting cur->next
        cur->next = pred; // cur points to pred
        pred = cur; // first step pred
        cur = succ; // second step cur
    }
    return pred; // cur is null, pred is new rev head
}

// recursive, rev the remain list, hand rev head, T: O(n), S: O(n) call-stack
ListNode* reverseListRecur(ListNode* head) {
    if (!head || !head->next) { return head; } // base case

    ListNode* revHead = reverseListRecur(head->next);
    head->next->next = head; // revTail points to head
    head->next = nullptr; // head becomes new revTail
    return revHead;
}

// build a fresh list from vals (both versions relink nodes), reverse, read back
vector<int> run(bool rec, const vector<int>& vals) {
    vector<ListNode> nodes;   // storage for the list
    for (int v : vals) { nodes.push_back({v, nullptr}); }
    // link only after all push_backs, so no reallocation moves a linked node
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    ListNode* head = n > 0 ? &nodes[0] : nullptr;
    head = rec ? reverseListRecur(head) : reverseList(head);
    vector<int> got;
    for (ListNode* cur = head; cur; cur = cur->next) { got.push_back(cur->val); }
    return got;
}

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    bool ok = run(false, vals) == want && run(true, vals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3}, {3, 2, 1});
    test("one", {1}, {1});
    test("empty", {}, {});
}
