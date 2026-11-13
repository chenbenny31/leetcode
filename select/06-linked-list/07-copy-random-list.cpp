// LC 138 Copy List with Random Pointer (Medium) — original -> copy hash map
// node has next + random (any node or null); return deep copy
// no pointer of the copy may point into the original
//
// clarify:
// random null or self -> ok, map handles both
// dup values -> key by pointer, not value
// original -> must end up unchanged
// O(1) extra space -> follow-up (weave)

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

// core idea: pass 1 copies every node, pass 2 wires next + random via the map
// T: O(n), S: O(n)
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

vector<int> encode(Node* head) {
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

bool checkAndFree(Node* head, Node* copy) {
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

void test(const string& label, const vector<int>& vals, const vector<int>& rnd) {
    int n = vals.size();
    vector<Node> pool;
    for (int v : vals) { pool.push_back(Node(v)); }
    for (int i = 0; i < n; i++) {
        pool[i].next = i + 1 < n ? &pool[i + 1] : nullptr;
        pool[i].random = rnd[i] >= 0 ? &pool[rnd[i]] : nullptr;
    }
    Node* head = n > 0 ? &pool[0] : nullptr;
    bool ok = checkAndFree(head, copyRandomList(head));
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("random-links", {7, 13, 11}, {2, -1, 0});
    test("self-random", {5}, {0});
    test("dup-values", {3, 3, 3}, {-1, 0, -1});
    test("empty", {}, {});
    return 0;
}

// follow-up:
// O(1) extra space -> weave A -> A' -> B -> B'; X'.random = X.random->next; unweave
// one pass -> getCopy(node) creates copy on first use; call for cur, next, random
// clone graph (LC 133) -> 05-clone-graph: same original -> copy map, filled in DFS / BFS
// copy binary tree w/ random (LC 1485) -> same map over a tree walk
