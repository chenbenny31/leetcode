// LC 23 Merge k Sorted Lists (Hard). Pattern: min-heap of list heads.
// Merge k sorted linked lists into one sorted list and return its head.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;

struct ListNode {
    int val;
    ListNode* next;
};

// Min-heap holds the current head of each list; pop the smallest, push its successor.
// T: O(N log k) for N nodes in total, S: O(k).
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    // heads[i] = next unmerged node of list i
    vector<ListNode*> heads = lists;
    int k = heads.size();
    // (val, list index) of each non-empty list's head, smallest val on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (int i = 0; i < k; i++) {
        if (heads[i]) { heap.push({heads[i]->val, i}); }
    }
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    while (!heap.empty()) {
        int listIdx = heap.top().second;
        heap.pop();
        tail->next = heads[listIdx];
        tail = heads[listIdx];
        heads[listIdx] = heads[listIdx]->next;
        // refill from the list we just took from
        if (heads[listIdx]) { heap.push({heads[listIdx]->val, listIdx}); }
    }
    return dummy.next;
}

void test(const string& label, const vector<ListNode*>& lists, const vector<int>& want) {
    vector<int> got;
    for (ListNode* cur = mergeKLists(lists); cur; cur = cur->next) {
        got.push_back(cur->val);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    ListNode a2{4, nullptr};
    ListNode a1{1, &a2};
    ListNode b2{3, nullptr};
    ListNode b1{2, &b2};
    ListNode c2{5, nullptr};
    ListNode c1{0, &c2};
    test("three-lists", {&a1, &b1, &c1}, {0, 1, 2, 3, 4, 5});
    ListNode d2{1, nullptr};
    ListNode d1{1, &d2};
    ListNode e1{1, nullptr};
    test("dups-and-empty", {&d1, nullptr, &e1}, {1, 1, 1});
    test("no-lists", {}, {});
}

// follow-up: no heap -> divide and conquer: merge lists in pairs, then pairs of
//   results, log k rounds of O(N) each; O(N log k) time, O(1) extra if iterative
