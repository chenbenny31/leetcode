// LC 1650 LCA with parent pointers: nodes know their parent (no root given). Return the
// lowest common ancestor of p and q (a node may be its own ancestor).

#include <iostream>
#include <string>
#include <unordered_set>

using std::cout;
using std::string;
using std::unordered_set;

struct Node {
    int val;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    Node(int v) : val(v) {}
};

// Store p and all its ancestors, then climb from q to the first stored node.
// T: O(h), S: O(h).
Node* lowestCommonAncestor(Node* p, Node* q) {
    // p and every ancestor of p
    unordered_set<Node*> ancestors;
    for (Node* a = p; a != nullptr; a = a->parent) { ancestors.insert(a); }
    Node* b = q;
    while (!ancestors.count(b)) { b = b->parent; }
    return b;
}

// follow-up, O(1) space: two pointers that jump to the other start when they reach the
// top meet at the LCA (both walk depth(p) + depth(q) steps). T: O(h), S: O(1).
Node* lowestCommonAncestorTwoPointers(Node* p, Node* q) {
    Node* a = p;
    Node* b = q;
    while (a != b) {
        if (a == nullptr) {
            a = q;
        } else {
            a = a->parent;
        }
        if (b == nullptr) {
            b = p;
        } else {
            b = b->parent;
        }
    }
    return a;
}

void test(const string& label, Node* p, Node* q, Node* want) {
    bool ok = lowestCommonAncestor(p, q) == want &&
              lowestCommonAncestorTwoPointers(p, q) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // 3 has children 5, 1; 5 has children 6, 2
    Node a(3), b(5), c(1), d(6), e(2);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    b.parent = &a;
    c.parent = &a;
    d.parent = &b;
    e.parent = &b;
    test("split-at-root", &d, &c, &a);
    test("ancestor-of-other", &b, &e, &b);
    test("siblings", &d, &e, &b);
}
