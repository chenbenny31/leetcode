// LC 19 Remove Nth Node From End of List: remove the n-th node from the end of the
// list (1 <= n <= length) and return its head.
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

// Two pointers n apart: when fast stands on the last node, slow stands just before the
// target. The dummy head makes removing the real head uniform. T: O(L), S: O(1).
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* fast = &dummy;
    ListNode* slow = &dummy;   // node right before the one to remove
    for (int i = 0; i < n; i++) {
        fast = fast->next;
    }
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }
    slow->next = slow->next->next;
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

void test(const string& label, const vector<int>& vals, int n, const vector<int>& want) {
    vector<ListNode> pool;
    cout << label << (toVec(removeNthFromEnd(build(pool, vals), n)) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("last", {1, 2, 3}, 1, {1, 2});
    test("head", {1, 2, 3}, 3, {2, 3});
    test("only-node", {1}, 1, {});
}
