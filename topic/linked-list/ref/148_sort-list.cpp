// LC 148 Sort List: sort a singly linked list in ascending order.
// Follow-up: O(n log n) time and O(1) memory.
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

// Cut off the first n nodes of head and return the rest. T: O(n), S: O(1).
ListNode* split(ListNode* head, int n) {
    for (int i = 1; head && i < n; i++) {
        head = head->next;
    }
    if (!head) { return nullptr; }
    ListNode* rest = head->next;
    head->next = nullptr;
    return rest;
}

// Merge sorted a and b after tail; return the last node of the result.
// T: O(|a| + |b|), S: O(1).
ListNode* mergeOnto(ListNode* a, ListNode* b, ListNode* tail) {
    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    while (tail->next) {
        tail = tail->next;
    }
    return tail;
}

// Bottom-up merge sort: merge runs of length step = 1, 2, 4, ... in place, so no
// recursion stack. T: O(L log L), S: O(1).
ListNode* sortList(ListNode* head) {
    int len = 0;
    for (ListNode* cur = head; cur; cur = cur->next) {
        len++;
    }
    ListNode dummy(0);
    dummy.next = head;
    for (int step = 1; step < len; step *= 2) {
        ListNode* tail = &dummy;   // last node of the part merged in this pass
        ListNode* cur = dummy.next;
        while (cur) {
            ListNode* left = cur;
            ListNode* right = split(left, step);
            cur = split(right, step);
            tail = mergeOnto(left, right, tail);
        }
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

void test(const string& label, const vector<int>& vals, const vector<int>& want) {
    vector<ListNode> pool;
    cout << label << (toVec(sortList(build(pool, vals))) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even-length", {4, 2, 1, 3}, {1, 2, 3, 4});
    test("odd-dups", {3, 1, 3, 2, 1}, {1, 1, 2, 3, 3});
    test("single", {1}, {1});
    test("empty", {}, {});
}
