// LC 23 Merge k Sorted Lists (Hard) — min-heap of list heads / divide and conquer
// merge k ascending linked lists into one ascending list, reusing nodes; return head
//
// clarify:
// empty lists / k = 0 -> skip / return null
// dup -> kept
// reuse nodes or allocate? -> reuse
// ranges -> k <= 1e4, N <= 1e4 nodes total

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

// solution 1: min-heap of heads; also works for streams / iterators
// core idea: heap holds cur head of each list; pop smallest, append, push its next
// T: O(N log k), S: O(k)
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    // heads[i] = next unmerged node of list i
    vector<ListNode*> heads = lists;
    int k = heads.size();
    // (val, list idx) of each non-empty list's head, smallest val on top
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

// merges two sorted lists, returns head
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

// merges lists[lo..hi] (inclusive), returns head
ListNode* mergeRange(const vector<ListNode*>& lists, int lo, int hi) {
    if (lo > hi) { return nullptr; }
    if (lo == hi) { return lists[lo]; }
    int mid = lo + (hi - lo) / 2;
    ListNode* left = mergeRange(lists, lo, mid);
    ListNode* right = mergeRange(lists, mid + 1, hi);
    return mergeTwo(left, right);
}

// solution 2: divide and conquer; no heap, less extra space
// core idea: merge each half of the lists, then merge the two results; log k levels
// T: O(N log k), S: O(log k) — recursion
ListNode* mergeKListsDivide(const vector<ListNode*>& lists) {
    return mergeRange(lists, 0, (int)lists.size() - 1);
}

// one list per row; pool owns the nodes (deque never moves them)
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

// follow-up:
// merge two sorted lists (LC 21) -> mergeTwo above: dummy head, take the smaller head
// O(1) extra space -> pair up iteratively: lists[i] w/ lists[i + step], step = 1, 2, 4
// k sorted arrays / iterators -> same heap w/ (value, source idx, position)
// external sort (inputs too big for memory) -> heap over k file readers, write in chunks
// smallest range covering all lists (LC 632) -> same heap + cur max; advance list w/ min
// design Twitter feed (LC 355) -> k-way merge of followees' tweets, max-heap by time
