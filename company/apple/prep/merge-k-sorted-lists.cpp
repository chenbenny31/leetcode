// merge k sorted linked list into one, return its head

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

// merge two-list, T: O(m+n), S: O(1)
ListNode* mergeTwo(ListNode* a, ListNode* b) {
    ListNode dummy{0, nullptr};
    ListNode* build = &dummy;
    while (a && b) {
        if (a->val <= b->val) {
            build->next = a;
            a = a->next;
        } else {
            build->next = b;
            b = b->next;
        }
        build = build->next;
    }
    build->next = a ? a : b;
    return dummy.next;
}

// merge k lists by recursively call mergeTwo, T: O(nk), S: O(1), n = total-nodes
ListNode* mergeKLists(const vector<ListNode*>& lists) {
    ListNode* head = nullptr;
    for (ListNode* list : lists) {
        head = mergeTwo(head, list);
    }
    return head;
}

// min-heap hold cur head of each list, each pick cost logk, T: (nlogk), S: O(k)
ListNode* mergeKListsHeap(const vector<ListNode*>& lists) {
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> heap; // minHeap, top by head's val
    vector<ListNode*> heads = lists; // heads[i] = next un-merged node of list i
    int k = heads.size();
    for (int i = 0; i < k; i++) {
        if (heads[i]) { heap.push({heads[i]->val, i}); }
    }

    ListNode dummy{0, nullptr};
    ListNode* build = &dummy;
    while (!heap.empty()) {
        int i = heap.top().second;
        heap.pop();
        build->next = heads[i];
        build = build->next;
        heads[i] = heads[i]->next;
        if (heads[i]) { heap.push({heads[i]->val, i}); }
    }
    return dummy.next;
}

// build fresh lists from vals (both merges relink nodes), merge, read back the values
vector<int> run(bool useHeap, const vector<vector<int>>& vals) {
    int k = vals.size();
    vector<vector<ListNode>> store(k);   // store[i] = nodes of list i
    vector<ListNode*> lists(k, nullptr);
    for (int i = 0; i < k; i++) {
        for (int v : vals[i]) { store[i].push_back({v, nullptr}); }
        // link only after all push_backs, so no reallocation moves a linked node
        int len = store[i].size();
        for (int j = 0; j + 1 < len; j++) { store[i][j].next = &store[i][j + 1]; }
        if (len > 0) { lists[i] = &store[i][0]; }
    }
    ListNode* head = useHeap ? mergeKListsHeap(lists) : mergeKLists(lists);
    vector<int> got;
    for (ListNode* cur = head; cur; cur = cur->next) { got.push_back(cur->val); }
    return got;
}

void test(const string& label, const vector<vector<int>>& vals, const vector<int>& want) {
    bool ok = run(false, vals) == want && run(true, vals) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {{1, 3}, {1, 2}, {4}}, {1, 1, 2, 3, 4});
    test("empty-one", {{}, {1}}, {1});
    test("none", {}, {});
}
