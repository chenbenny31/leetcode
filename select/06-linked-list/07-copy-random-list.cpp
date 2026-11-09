// LC 138 Copy List with Random Pointer (Medium). Pattern: old -> new node hash map.
// Each node has next and random (any node or null). Return a deep copy; no pointer of
// the copy may point into the original.

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cout;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int x) : val(x), next(nullptr), random(nullptr) {}
};

// Pass 1 creates every copy, pass 2 wires next and random through the map.
// T: O(n), S: O(n).
Node* copyRandomList(Node* head) {
    // original node -> its copy; nullptr -> nullptr
    unordered_map<Node*, Node*> copyOf;
    copyOf[nullptr] = nullptr;
    for (Node* cur = head; cur; cur = cur->next) { copyOf[cur] = new Node(cur->val); }
    for (Node* cur = head; cur; cur = cur->next) {
        copyOf[cur]->next = copyOf[cur->next];
        copyOf[cur]->random = copyOf[cur->random];
    }
    return copyOf[head];
}

// Follow-up O(1) extra: weave A -> A' -> B -> B', so X.random's copy is X.random->next,
// then unweave and restore the original. T: O(n), S: O(1).
Node* copyRandomListInPlace(Node* head) {
    for (Node* cur = head; cur; cur = cur->next->next) {
        Node* copy = new Node(cur->val);
        copy->next = cur->next;
        cur->next = copy;
    }
    for (Node* cur = head; cur; cur = cur->next->next) {
        if (cur->random) { cur->next->random = cur->random->next; }
    }
    Node dummy(0);
    // last node of the copied list
    Node* tail = &dummy;
    for (Node* cur = head; cur; cur = cur->next) {
        Node* copy = cur->next;
        cur->next = copy->next;
        tail->next = copy;
        tail = copy;
    }
    return dummy.next;
}

// list -> {val, index its random points to or -1, ...}; -2 if random leaves the list
vector<int> encode(Node* head) {
    // node -> its position in this list
    unordered_map<Node*, int> pos;
    int index = 0;
    for (Node* cur = head; cur; cur = cur->next) {
        pos[cur] = index;
        index++;
    }
    vector<int> out;
    for (Node* cur = head; cur; cur = cur->next) {
        out.push_back(cur->val);
        if (!cur->random) {
            out.push_back(-1);
        } else {
            out.push_back(pos.count(cur->random) ? pos[cur->random] : -2);
        }
    }
    return out;
}

// copy has the same shape as head and shares no node with it; frees copy
bool checkAndFree(Node* head, Node* copy) {
    // nodes of the original list
    unordered_set<Node*> original;
    for (Node* cur = head; cur; cur = cur->next) { original.insert(cur); }
    bool ok = encode(copy) == encode(head);
    for (Node* cur = copy; cur; cur = cur->next) {
        if (original.count(cur)) { ok = false; }
    }
    while (ok && copy) {
        Node* nxt = copy->next;
        delete copy;
        copy = nxt;
    }
    return ok;
}

// rnd[i] = index node i's random points to, -1 for null
void test(const string& label, const vector<int>& vals, const vector<int>& rnd) {
    int n = vals.size();
    vector<Node> pool;
    for (int v : vals) { pool.push_back(Node(v)); }
    for (int i = 0; i < n; i++) {
        pool[i].next = i + 1 < n ? &pool[i + 1] : nullptr;
        pool[i].random = rnd[i] >= 0 ? &pool[rnd[i]] : nullptr;
    }
    Node* head = n > 0 ? &pool[0] : nullptr;
    bool ok = checkAndFree(head, copyRandomList(head))
              && checkAndFree(head, copyRandomListInPlace(head));
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("random-links", {7, 13, 11}, {2, -1, 0});
    test("self-random", {5}, {0});
    test("dup-values", {3, 3, 3}, {-1, 0, -1});
    test("empty", {}, {});
}

// follow-up: clone graph (LC 133) -> same original -> copy map, filled during DFS / BFS
