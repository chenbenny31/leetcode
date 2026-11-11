// LC 25 Reverse Nodes in k-Group (Hard). Pattern: count, then reverse group by group.
// Reverse the list k nodes at a time; a last group with fewer than k nodes stays as is.
// Only next pointers may change. 1 <= k <= list length.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Count n, then reverse n / k full groups, each hooked after prevTail. T: O(n), S: O(1).
ListNode* reverseKGroup(ListNode* head, int k) {
    int n = 0;
    for (ListNode* cur = head; cur; cur = cur->next) { n++; }
    ListNode dummy(0);
    dummy.next = head;
    // last node of the part already done
    ListNode* prevTail = &dummy;
    for (int group = 0; group < n / k; group++) {
        ListNode* groupHead = prevTail->next;
        ListNode* prev = nullptr;
        ListNode* cur = groupHead;
        for (int i = 0; i < k; i++) {
            ListNode* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }
        // prev = new group head, groupHead = new group tail, cur = next group's head
        prevTail->next = prev;
        groupHead->next = cur;
        prevTail = groupHead;
    }
    return dummy.next;
}

// pool owns the nodes; links them in order and returns the head
ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

vector<int> toVec(ListNode* head) {
    vector<int> out;
    for (ListNode* cur = head; cur; cur = cur->next) { out.push_back(cur->val); }
    return out;
}

void test(const string& label, const vector<int>& vals, int k, const vector<int>& want) {
    vector<ListNode> pool;
    ListNode* head = reverseKGroup(build(pool, vals), k);
    cout << label << (toVec(head) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("k-one", {1, 2, 3}, 1, {1, 2, 3});
    test("whole-list", {1, 2, 3}, 3, {3, 2, 1});
    test("even-groups", {1, 2, 3, 4, 5, 6}, 3, {3, 2, 1, 6, 5, 4});
    test("short-tail", {1, 2, 3, 4, 5, 6}, 4, {4, 3, 2, 1, 5, 6});
}

// follow-up: reverse the short tail too -> one more reversal of the last n % k nodes
// follow-up: recursive -> reverse the first k, groupHead->next = reverseKGroup(rest, k),
//   O(n / k) stack
