// LC 23 Merge k Sorted Lists: given an array of k sorted linked lists, merge them into
// one sorted list and return its head.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
using std::cout;
using std::greater;
using std::pair;
using std::priority_queue;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Min-heap holds the current front of each non-empty list; pop the smallest, advance
// that list. T: O(N log k) for N total nodes, S: O(k).
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    int k = lists.size();
    vector<ListNode*> front = lists;   // front[i] = next unmerged node of list i
    // (value at front, list index)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    for (int i = 0; i < k; i++) {
        if (front[i]) { heap.push({front[i]->val, i}); }
    }
    ListNode dummy(0);
    ListNode* tail = &dummy;   // last node of the merged list so far
    while (!heap.empty()) {
        int i = heap.top().second;
        heap.pop();
        tail->next = front[i];
        tail = tail->next;
        front[i] = front[i]->next;
        if (front[i]) { heap.push({front[i]->val, i}); }
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

void test(const string& label, const vector<vector<int>>& vals, const vector<int>& want) {
    int k = vals.size();
    vector<vector<ListNode>> pools(k);
    vector<ListNode*> lists;
    for (int i = 0; i < k; i++) {
        lists.push_back(build(pools[i], vals[i]));
    }
    cout << label << (toVec(mergeKLists(lists)) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-lists", {{1, 4}, {2, 5}, {0, 3}}, {0, 1, 2, 3, 4, 5});
    test("with-empty", {{}, {1, 1}, {}}, {1, 1});
    test("no-lists", {}, {});
}
