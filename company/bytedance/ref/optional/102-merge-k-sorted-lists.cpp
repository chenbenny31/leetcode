// ByteDance / LC 23: merge k sorted linked lists into one sorted list and return its
// head.
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

// Min-heap holds the current head of each list. T: O(N log k), S: O(k).
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    // (val, list index) of each list's current head; min val on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    // heads[i] = next unmerged node of list i
    vector<ListNode*> heads = lists;
    int k = heads.size();
    for (int i = 0; i < k; i++) {
        if (heads[i]) { heap.push({heads[i]->val, i}); }
    }
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    while (!heap.empty()) {
        int i = heap.top().second;
        heap.pop();
        tail->next = heads[i];
        tail = heads[i];
        heads[i] = heads[i]->next;
        // refill from the list we just took from
        if (heads[i]) { heap.push({heads[i]->val, i}); }
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
    ListNode a3{5, nullptr};
    ListNode a2{4, &a3};
    ListNode a1{1, &a2};
    ListNode b3{4, nullptr};
    ListNode b2{3, &b3};
    ListNode b1{1, &b2};
    ListNode c2{6, nullptr};
    ListNode c1{2, &c2};
    test("lc-ex1", {&a1, &b1, &c1}, {1, 1, 2, 3, 4, 4, 5, 6});
    test("empty-lists", {nullptr, nullptr}, {});
    test("no-lists", {}, {});
}

// follow-up: no heap -> divide & conquer, merge pairs (LC 21) with step 1, 2, 4, ...,
//   O(N log k) time, O(1) extra
// follow-up: why not merge one by one -> early nodes get re-walked up to k times, O(N*k)
// follow-up: k sorted arrays -> heap of (val, array idx, elem idx), O(N log k)
// follow-up: lists too big for memory -> external k-way merge: buffered reader per file,
//   heap of k heads, write output in blocks
