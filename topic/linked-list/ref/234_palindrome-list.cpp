// LC 234 Palindrome Linked List: return true if the list reads the same forwards and
// backwards.
// Follow-up: O(n) time and O(1) space.
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

// Reverse the second half in place (O(1) space), then compare it with the first half.
// The list is left half-reversed; reverse again to restore it if the caller needs it.
// T: O(L), S: O(1).
bool isPalindrome(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    // slow is the middle (odd length) or first node of the second half (even length)
    ListNode* back = nullptr;   // reversed second half
    while (slow) {
        ListNode* nxt = slow->next;
        slow->next = back;
        back = slow;
        slow = nxt;
    }
    ListNode* front = head;
    while (back) {
        if (front->val != back->val) { return false; }
        front = front->next;
        back = back->next;
    }
    return true;
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

void test(const string& label, const vector<int>& vals, bool want) {
    vector<ListNode> pool;
    cout << label << (isPalindrome(build(pool, vals)) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("even", {1, 2, 2, 1}, true);
    test("odd", {1, 2, 3, 2, 1}, true);
    test("not", {1, 2, 3}, false);
    test("single", {7}, true);
}
