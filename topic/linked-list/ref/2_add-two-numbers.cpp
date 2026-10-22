// LC 2 Add Two Numbers: two non-empty lists hold non-negative integers, one digit per
// node, least significant digit first. Return their sum as a list in the same format.
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

// Walk both lists with a carry; a dummy head avoids the first-node special case.
// T: O(max(m, n)), S: O(max(m, n)) for the result.
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;   // last node of the result so far
    int carry = 0;
    while (l1 || l2 || carry > 0) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        carry = sum / 10;
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

void test(const string& label, const vector<int>& a, const vector<int>& b,
          const vector<int>& want) {
    vector<ListNode> pa, pb;
    ListNode* sum = addTwoNumbers(build(pa, a), build(pb, b));
    cout << label << (toVec(sum) == want ? " PASS\n" : " FAIL\n");
    while (sum) {
        ListNode* nxt = sum->next;
        delete sum;
        sum = nxt;
    }
}

int main() {
    test("uneven", {3, 4}, {5, 6, 7}, {8, 0, 8});
    test("final-carry", {9, 9}, {1}, {0, 0, 1});
    test("zeros", {0}, {0}, {0});
}
