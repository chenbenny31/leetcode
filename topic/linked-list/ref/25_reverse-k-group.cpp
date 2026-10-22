// LC 25 Reverse Nodes in k-Group: reverse the list k nodes at a time; a final group
// shorter than k stays as is. Only node links may change, not values.
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

// For each group: find its last node (stop if fewer than k remain), reverse the group
// in place splicing it back between prev and after. T: O(L), S: O(1).
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;   // last node of the already finished part
    while (true) {
        ListNode* last = prev;   // last node of the current group
        for (int i = 0; i < k; i++) {
            last = last->next;
            if (!last) { return dummy.next; }
        }
        ListNode* first = prev->next;
        ListNode* after = last->next;   // first node of the next group
        ListNode* rev = after;          // reversed prefix points on to the next group
        ListNode* cur = first;
        while (cur != after) {
            ListNode* nxt = cur->next;
            cur->next = rev;
            rev = cur;
            cur = nxt;
        }
        prev->next = last;
        prev = first;
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

void test(const string& label, const vector<int>& vals, int k, const vector<int>& want) {
    vector<ListNode> pool;
    cout << label << (toVec(reverseKGroup(build(pool, vals), k)) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("short-tail", {1, 2, 3, 4, 5}, 3, {3, 2, 1, 4, 5});
    test("exact-groups", {1, 2, 3, 4}, 2, {2, 1, 4, 3});
    test("k-one", {1, 2, 3}, 1, {1, 2, 3});
    test("k-too-big", {1, 2}, 3, {1, 2});
}
