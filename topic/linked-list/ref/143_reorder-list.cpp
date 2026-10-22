// LC 143 Reorder List: reorder L0 -> L1 -> ... -> Ln into L0 -> Ln -> L1 -> Ln-1 -> ...
// in place by relinking nodes, without changing values.
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

// Find the middle with slow/fast, reverse the second half, then interleave the two
// halves. T: O(L), S: O(1).
void reorderList(ListNode* head) {
    if (!head || !head->next) { return; }
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    ListNode* second = nullptr;   // reversed second half
    ListNode* cur = slow->next;
    slow->next = nullptr;
    while (cur) {
        ListNode* nxt = cur->next;
        cur->next = second;
        second = cur;
        cur = nxt;
    }
    ListNode* first = head;
    while (second) {
        ListNode* firstNext = first->next;
        ListNode* secondNext = second->next;
        first->next = second;
        second->next = firstNext;
        first = firstNext;
        second = secondNext;
    }
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

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    reorderList(head);
    cout << label << (toVec(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd", {1, 2, 3, 4, 5}, {1, 5, 2, 4, 3});
    test("even", {1, 2, 3, 4}, {1, 4, 2, 3});
    test("two", {1, 2}, {1, 2});
    test("single", {1}, {1});
}
