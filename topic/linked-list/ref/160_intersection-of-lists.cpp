// LC 160 Intersection of Two Linked Lists: return the node where two singly linked lists
// start to share nodes (same node, not same value), or null if they never meet.
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

// Each pointer walks its list then the other one, so both travel lenA + lenB and meet
// at the intersection, or both hit null together. T: O(m + n), S: O(1).
ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
    ListNode* a = headA;
    ListNode* b = headB;
    while (a != b) {
        a = a ? a->next : headB;
        b = b ? b->next : headA;
    }
    return a;
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

void test(const string& label, const vector<int>& onlyA, const vector<int>& onlyB,
          const vector<int>& shared) {
    vector<ListNode> pa, pb, ps;
    ListNode* common = build(ps, shared);
    ListNode* a = build(pa, onlyA, common);
    ListNode* b = build(pb, onlyB, common);
    cout << label << (getIntersectionNode(a, b) == common ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unequal-prefix", {1, 2}, {5}, {8, 9});
    test("disjoint", {1, 2}, {3}, {});
    test("a-inside-b", {}, {5}, {8});
}
