// LC 328 Odd Even Linked List: group all nodes at odd positions first, then those at
// even positions, keeping relative order; use O(1) extra space.
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

// Build the odd chain and the even chain in one pass, then link odd tail to even head.
// T: O(L), S: O(1).
ListNode* oddEvenList(ListNode* head) {
    if (!head) { return head; }
    ListNode* odd = head;               // tail of the odd chain
    ListNode* evenHead = head->next;
    ListNode* even = evenHead;          // tail of the even chain
    while (even && even->next) {
        odd->next = even->next;
        odd = odd->next;
        even->next = odd->next;
        even = even->next;
    }
    odd->next = evenHead;
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

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> pool;
    cout << label << (toVec(oddEvenList(build(pool, vals))) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("odd-length", {1, 2, 3, 4, 5}, {1, 3, 5, 2, 4});
    test("even-length", {1, 2, 3, 4}, {1, 3, 2, 4});
    test("single", {1}, {1});
    test("empty", {}, {});
}
