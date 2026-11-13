// LC 23 Merge k Sorted Lists (Hard) — min-heap of list heads / divide and conquer
// Merge k ascending linked lists into one ascending list, reusing the nodes, and return
// its head.
// Clarify: empty lists and k = 0; duplicates; reuse the nodes or allocate new ones;
//   ranges (k <= 1e4, N <= 1e4 nodes in total).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::deque;
using std::greater;

struct ListNode {
    int val;
    ListNode* next;
};

// Core idea: a min-heap holds the current head of each list; pop the smallest, append
// it, push its successor. T: O(N log k) for N nodes in total, S: O(k).
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

// merges two sorted lists and returns the head
ListNode* mergeTwo(ListNode* left, ListNode* right) {
    ListNode dummy{0, nullptr};
    ListNode* tail = &dummy;
    while (left && right) {
        if (left->val <= right->val) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    tail->next = left ? left : right;
    return dummy.next;
}

// merges lists[lo..hi] (inclusive) and returns the head
ListNode* mergeRange(const vector<ListNode*>& lists, int lo, int hi) {
    if (lo > hi) { return nullptr; }
    if (lo == hi) { return lists[lo]; }
    int mid = lo + (hi - lo) / 2;
    ListNode* left = mergeRange(lists, lo, mid);
    ListNode* right = mergeRange(lists, mid + 1, hi);
    return mergeTwo(left, right);
}

// Core idea: divide and conquer: merge each half of the lists, then merge the two
// results; every node is touched once per level and there are log k levels.
// T: O(N log k), S: O(log k) recursion.
ListNode* mergeKListsDivide(const vector<ListNode*>& lists) {
    return mergeRange(lists, 0, (int)lists.size() - 1);
}

// builds one list per row; pool owns the nodes (a deque never moves them)
vector<ListNode*> buildLists(const vector<vector<int>>& rows, deque<ListNode>& pool) {
    vector<ListNode*> heads;
    for (const vector<int>& row : rows) {
        ListNode* head = nullptr;
        for (int i = (int)row.size() - 1; i >= 0; i--) {
            pool.push_back({row[i], head});
            head = &pool.back();
        }
        heads.push_back(head);
    }
    return heads;
}

vector<int> toVector(ListNode* head) {
    vector<int> values;
    for (ListNode* cur = head; cur; cur = cur->next) { values.push_back(cur->val); }
    return values;
}

void test(const string& label, const vector<vector<int>>& rows, const vector<int>& want) {
    deque<ListNode> pool;
    vector<int> byHeap = toVector(mergeKLists(buildLists(rows, pool)));
    vector<int> byDivide = toVector(mergeKListsDivide(buildLists(rows, pool)));
    cout << label << (byHeap == want && byDivide == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-lists", {{1, 4}, {2, 3}, {0, 5}}, {0, 1, 2, 3, 4, 5});
    test("dups-and-empty", {{1, 1}, {}, {1}}, {1, 1, 1});
    test("single-list", {{2, 6, 9}}, {2, 6, 9});
    test("no-lists", {}, {});
    return 0;
}

// follow-up (ideas only):
// - merge two sorted lists (LC 21) -> mergeTwo above: dummy head, take the smaller head
// - O(1) extra space -> iterative pairing: merge lists[i] with lists[i + step] for
//   step = 1, 2, 4, ... instead of recursing
// - k sorted arrays or iterators -> the same heap with (value, source index, position)
// - inputs too big for memory (external sort) -> the heap over k file readers, write
//   the output in chunks
// - smallest range covering one element from each list (LC 632) -> the same heap of
//   heads plus the current max; shrink by advancing the list that holds the min
// - design Twitter news feed (LC 355) -> k-way merge of the followees' tweet lists from
//   the newest end with a max-heap of (time, user, index); stop after 10 pops
