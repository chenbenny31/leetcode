// LC 61 Rotate List: rotate the list to the right by k places and return the new head.
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

// Close the list into a ring, then cut it after node len - k % len. T: O(L), S: O(1).
ListNode* rotateRight(ListNode* head, int k) {
    if (!head) { return head; }
    int len = 1;
    ListNode* tail = head;
    while (tail->next) {
        tail = tail->next;
        len++;
    }
    k %= len;
    tail->next = head;
    // with k == 0 this walks to the old tail and the cut changes nothing
    ListNode* newTail = head;
    for (int i = 0; i < len - k - 1; i++) {
        newTail = newTail->next;
    }
    head = newTail->next;
    newTail->next = nullptr;
    return head;
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
    cout << label << (toVec(rotateRight(build(pool, vals), k)) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("rotate", {1, 2, 3, 4, 5}, 2, {4, 5, 1, 2, 3});
    test("k-multiple-of-len", {1, 2}, 4, {1, 2});
    test("k-over-len", {1, 2, 3}, 4, {3, 1, 2});
    test("empty", {}, 3, {});
}
