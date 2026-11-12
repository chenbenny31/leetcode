// LC 142 Linked List Cycle II (Medium). Pattern: Floyd fast/slow pointers.
// Return the node where the cycle begins, or null if the list has no cycle. O(1) space.

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

// After slow and fast meet, a walker from head and one from the meeting point reach the
// entry together. T: O(n), S: O(1).
ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // slow walked a + b, fast 2(a + b): a + b is a multiple of the cycle length,
            // so a steps from the meeting point land on the entry
            ListNode* cur = head;
            while (cur != slow) {
                cur = cur->next;
                slow = slow->next;
            }
            return cur;
        }
    }
    return nullptr;
}

// LC 287: n + 1 values in [1, n]. Index i is a node, nums[i] its next pointer; no
// value is 0, so index 0 is the head outside the cycle, and the duplicate value is a
// node with two incoming pointers = the cycle entry. T: O(n), S: O(1).
int findDuplicate(const vector<int>& nums) {
    // slow = slow->next becomes slow = nums[slow]
    int slow = nums[0];
    int fast = nums[nums[0]];
    while (slow != fast) {
        slow = nums[slow];
        fast = nums[nums[fast]];
    }
    // same as detectCycle: walkers from the head and the meeting point meet at the entry
    int cur = 0;
    while (cur != slow) {
        cur = nums[cur];
        slow = nums[slow];
    }
    return cur;
}

// pool owns the nodes; links them in order and returns the head
ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

// pos = index the last node points back to, -1 for no cycle; want = that node
void test(const string& label, const vector<int>& vals, int pos) {
    vector<ListNode> pool;
    ListNode* head = build(pool, vals);
    ListNode* want = nullptr;
    if (pos >= 0) {
        pool.back().next = &pool[pos];
        want = &pool[pos];
    }
    cout << label << (detectCycle(head) == want ? " PASS\n" : " FAIL\n");
}

void testDup(const string& label, const vector<int>& nums, int want) {
    cout << label << (findDuplicate(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("entry-middle", {1, 2, 3, 4, 5}, 2);
    test("entry-head", {1, 2, 3}, 0);
    test("self-loop", {1, 2, 3}, 2);
    test("no-cycle", {1, 2, 3}, -1);
    testDup("dup-long-cycle", {1, 2, 3, 4, 2}, 2);
    testDup("dup-self-loop", {2, 1, 3, 1}, 1);
    testDup("dup-many-copies", {2, 2, 2, 2}, 2);
    testDup("dup-smallest", {1, 1}, 1);
}

// follow-up: cycle length -> after meeting, step slow around until it is back at fast
// follow-up: O(n) space allowed -> unordered_set of visited nodes; first repeat = entry
