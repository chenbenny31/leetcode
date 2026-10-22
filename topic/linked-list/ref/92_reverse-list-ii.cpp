// LC 92 Reverse Linked List II: reverse the nodes from position left to right
// (1-indexed, left <= right) and return the head.
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

// Head insertion: the node at left stays put while each following node in the range is
// moved to just after prev. The dummy head covers left == 1. T: O(L), S: O(1).
ListNode* reverseBetween(ListNode* head, int left, int right) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;   // node right before position left
    for (int i = 1; i < left; i++) {
        prev = prev->next;
    }
    ListNode* cur = prev->next;   // starts at left, ends up last in the reversed range
    for (int i = left; i < right; i++) {
        ListNode* nxt = cur->next;
        cur->next = nxt->next;
        nxt->next = prev->next;
        prev->next = nxt;
    }
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

void test(const string& label, const vector<int>& vals, int left, int right,
          const vector<int>& want) {
    vector<ListNode> pool;
    cout << label << (toVec(reverseBetween(build(pool, vals), left, right)) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {1, 2, 3, 4, 5}, 2, 4, {1, 4, 3, 2, 5});
    test("whole", {1, 2, 3}, 1, 3, {3, 2, 1});
    test("single-position", {1, 2}, 2, 2, {1, 2});
}
