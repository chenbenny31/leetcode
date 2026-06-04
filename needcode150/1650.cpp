// hash-set, T: O(h), S: O(h)

#include <unordered_set>
#include <cstddef>

class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        std::unordered_set<Node*> seen;
        while (p) {
            seen.insert(p);
            p = p->parent;
        }
        while (q) {
            if (seen.count(q)) { return q; }
            q = q->parent;
        }
        return nullptr;
    }
};

// two pointers, path equalization, T: O(h), S: O(1)

#include <cstddef>

class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        Node* a = p;
        Node* b = q;

        while (a != b) {
            a = a ? a->parent : q;
            b = b ? b->parent : p;
        }
        return a;
    }
};

// a != b: pointer comparison address not value
