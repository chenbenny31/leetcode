// ByteDance (hack2hire) / LC 116: in a perfect binary tree, point each node's next to
// its right neighbor on the same level (nullptr at the level's end), O(1) extra space.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;
};

// Wire level d+1 by walking level d through its next chain. T: O(n), S: O(1).
Node* connect(Node* root) {
    // first node of the level already wired
    Node* leftmost = root;
    while (leftmost && leftmost->left) {
        for (Node* cur = leftmost; cur; cur = cur->next) {
            cur->left->next = cur->right;
            // the right child's neighbor is the left child of cur's neighbor
            if (cur->next) {
                cur->right->next = cur->next->left;
            }
        }
        leftmost = leftmost->left;
    }
    return root;
}

// follow-up (LC 117, any binary tree): append each child of level d to a dummy-headed
// list for level d+1. T: O(n), S: O(1).
Node* connectAny(Node* root) {
    // first node of the level already wired
    Node* level = root;
    while (level) {
        // dummy.next = first node of the level below
        Node dummy{0, nullptr, nullptr, nullptr};
        // last node wired on the level below
        Node* tail = &dummy;
        for (Node* cur = level; cur; cur = cur->next) {
            if (cur->left) {
                tail->next = cur->left;
                tail = tail->next;
            }
            if (cur->right) {
                tail->next = cur->right;
                tail = tail->next;
            }
        }
        level = dummy.next;
    }
    return root;
}

// values along the next chain starting at node
vector<int> chain(Node* node) {
    vector<int> out;
    for (; node; node = node->next) {
        out.push_back(node->val);
    }
    return out;
}

void test(const string& label, Node* start, const vector<int>& want) {
    cout << label << (chain(start) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //        1
    //    2       3
    //  4   5   6   7
    Node n4{4, nullptr, nullptr, nullptr};
    Node n5{5, nullptr, nullptr, nullptr};
    Node n6{6, nullptr, nullptr, nullptr};
    Node n7{7, nullptr, nullptr, nullptr};
    Node n2{2, &n4, &n5, nullptr};
    Node n3{3, &n6, &n7, nullptr};
    Node n1{1, &n2, &n3, nullptr};
    connect(&n1);
    test("level-1", &n2, {2, 3});
    test("level-2", &n4, {4, 5, 6, 7});

    //      1
    //    2   3
    //  4       7
    Node m4{4, nullptr, nullptr, nullptr};
    Node m7{7, nullptr, nullptr, nullptr};
    Node m2{2, &m4, nullptr, nullptr};
    Node m3{3, nullptr, &m7, nullptr};
    Node m1{1, &m2, &m3, nullptr};
    connectAny(&m1);
    test("any:-gap", &m4, {4, 7});
}

// follow-up: any binary tree (LC 117) -> see connectAny() above; dummy head + tail
//   chains the next level
// follow-up: O(n) space OK -> BFS queue per level; node i < sz - 1 gets
//   next = q.front(), last gets nullptr
// follow-up: recursive, perfect tree -> link root->left->next, root->right->next via
//   root->next, recurse; O(log n) stack
