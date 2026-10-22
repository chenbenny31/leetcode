// LC 82 Remove Duplicates from Sorted List II: delete every value that appears more than
// once in a sorted list, keeping only values that occur exactly once.
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

// prev trails the last kept node; a node equal to its successor starts a run that is
// skipped whole. The dummy head covers a duplicate run at the front. T: O(L), S: O(1).
ListNode* deleteDuplicates(ListNode* head) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;   // last node known to be kept
    while (prev->next) {
        ListNode* cur = prev->next;
        if (!cur->next || cur->next->val != cur->val) {
            prev = cur;
            continue;
        }
        int dup = cur->val;
        while (cur && cur->val == dup) {
            cur = cur->next;
        }
        prev->next = cur;
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
    cout << label << (toVec(deleteDuplicates(build(pool, vals))) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("head-run", {1, 1, 2, 3, 3}, {2});
    test("tail-run", {1, 2, 2}, {1});
    test("all-same", {4, 4, 4}, {});
    test("no-dups", {1, 2, 3}, {1, 2, 3});
}
