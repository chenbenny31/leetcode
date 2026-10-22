// LC 206 Reverse Linked List: reverse a singly linked list and return the new head.
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

// Walk once, pointing each node back at the previous one. T: O(L), S: O(1).
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;   // head of the already reversed part
    ListNode* cur = head;
    while (cur) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
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
    cout << label << (toVec(reverseList(build(pool, vals))) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3}, {3, 2, 1});
    test("single", {1}, {1});
    test("empty", {}, {});
}
