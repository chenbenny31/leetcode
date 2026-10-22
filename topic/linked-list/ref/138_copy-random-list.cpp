// LC 138 Copy List with Random Pointer: each node has next and random (any node or
// null). Return a deep copy; no pointer of the copy may point into the original.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int x) : val(x), next(nullptr), random(nullptr) {}
};

// Interleave: weave each copy right after its original, so copy.random is just
// original.random->next; then unweave both lists. T: O(L), S: O(1) beyond the copy.
Node* copyRandomList(Node* head) {
    for (Node* cur = head; cur; cur = cur->next->next) {
        Node* copy = new Node(cur->val);
        copy->next = cur->next;
        cur->next = copy;
    }
    for (Node* cur = head; cur; cur = cur->next->next) {
        if (cur->random) { cur->next->random = cur->random->next; }
    }
    Node dummy(0);
    Node* tail = &dummy;   // last node of the copied list
    for (Node* cur = head; cur; cur = cur->next) {
        Node* copy = cur->next;
        cur->next = copy->next;
        tail->next = copy;
        tail = copy;
    }
    return dummy.next;
}

// rnd[i] = index that node i's random points to, or -1 for null
void test(const string& label, const vector<int>& vals, const vector<int>& rnd) {
    int n = vals.size();
    vector<Node> pool(n, Node(0));
    for (int i = 0; i < n; i++) {
        pool[i].val = vals[i];
        pool[i].next = i + 1 < n ? &pool[i + 1] : nullptr;
        pool[i].random = rnd[i] >= 0 ? &pool[rnd[i]] : nullptr;
    }
    Node* head = copyRandomList(n > 0 ? &pool[0] : nullptr);
    bool ok = true;
    Node* cur = head;
    for (int i = 0; i < n; i++) {
        if (!cur || cur == &pool[i] || cur->val != vals[i]) { ok = false; break; }
        // random must hit the copy of node rnd[i]: same value, but not the original
        if (rnd[i] < 0) {
            ok = ok && cur->random == nullptr;
        } else {
            ok = ok && cur->random && cur->random->val == vals[rnd[i]]
                 && cur->random != &pool[rnd[i]];
        }
        // the original list must be restored
        ok = ok && pool[i].next == (i + 1 < n ? &pool[i + 1] : nullptr);
        cur = cur->next;
    }
    ok = ok && cur == nullptr;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
    while (head) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

int main() {
    test("random-links", {7, 13, 11}, {2, -1, 0});
    test("self-random", {5}, {0});
    test("empty", {}, {});
}
