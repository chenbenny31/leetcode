// LC 206: reverse a singly linked list and return the new head.
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

// straightforward: iterative, flip each next pointer while walking. T: O(n), S: O(1).
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;   // head of the already reversed part
    ListNode* cur = head;
    while (cur) {
        ListNode* nxt = cur->next;   // save before overwriting cur->next
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
}

// recursive: reverse the rest first, then hang head after its old next node.
// T: O(n), S: O(n) call stack.
ListNode* reverseListRec(ListNode* head) {
    // empty or last node: it is the head of the reversed list
    if (!head || !head->next) { return head; }
    ListNode* newHead = reverseListRec(head->next);
    // head->next is now the tail of the reversed rest; append head after it
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

// build a fresh list from vals (both versions relink nodes), reverse, read back
vector<int> run(bool rec, const vector<int>& vals) {
    vector<ListNode> nodes;   // storage for the list
    for (int v : vals) { nodes.push_back({v, nullptr}); }
    // link only after all push_backs, so no reallocation moves a linked node
    int n = nodes.size();
    for (int i = 0; i + 1 < n; i++) { nodes[i].next = &nodes[i + 1]; }
    ListNode* head = n > 0 ? &nodes[0] : nullptr;
    head = rec ? reverseListRec(head) : reverseList(head);
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
