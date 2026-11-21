// 05 · Binary Linked List To Integer (Cisco OA, Easy) — LC 1290
// nodes hold 0 / 1, most significant bit first; return the value
//
// clarify:
// which end is the most significant bit -> head
// list len -> Cisco OA: N <= 64, so unsigned long long (LC 1290: <= 30, int)
// empty list -> 0

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

// core idea: reading a new bit shifts everything so far left by one: value * 2 + bit
// T: O(n), S: O(1)
unsigned long long getDecimalValue(ListNode* head) {
    unsigned long long value = 0;
    for (ListNode* cur = head; cur; cur = cur->next) {
        value = value * 2 + cur->val;
    }
    return value;
}

ListNode* build(vector<ListNode>& pool, const vector<int>& vals) {
    for (int v : vals) { pool.push_back(ListNode(v)); }
    int n = pool.size();
    for (int i = 0; i + 1 < n; i++) { pool[i].next = &pool[i + 1]; }
    return n > 0 ? &pool[0] : nullptr;
}

void test(const string& label, const vector<int>& bits, unsigned long long want) {
    vector<ListNode> pool;
    cout << label << (getDecimalValue(build(pool, bits)) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("101", {1, 0, 1}, 5);
    test("leading-zero", {0, 1, 1}, 3);
    test("single", {0}, 0);
    test("empty", {}, 0);
    test("64-ones", vector<int>(64, 1), ~0ULL);
    return 0;
}

// follow-up:
// more than 64 nodes -> return value % mod, taken at each step
// least significant bit first -> value += bit << i, i = idx of the node
// delete a node given only its pointer (LC 237, reported) -> copy next's val, unlink next
// reverse the list (LC 206, reported in C, both ways) -> select/06-linked-list/01
// has a cycle (LC 141, reported) -> select/06-linked-list/03, true at first meeting
