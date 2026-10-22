// LC 116 Populating Next Right Pointers in Each Node: in a perfect binary tree, set each
// node's next to its right neighbor on the same level (nullptr at the end of a level).
// Use O(1) extra space.
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
    Node(int v = 0) : val(v), left(nullptr), right(nullptr), next(nullptr) {}
};

// Walk a level through its already wired next chain and wire the level below it.
// T: O(n), S: O(1).
Node* connect(Node* root) {
    Node* leftmost = root;   // first node of the level whose next chain is wired
    while (leftmost != nullptr && leftmost->left != nullptr) {
        for (Node* cur = leftmost; cur != nullptr; cur = cur->next) {
            cur->left->next = cur->right;
            // across a parent boundary: right child -> neighbor parent's left child
            if (cur->next != nullptr) { cur->right->next = cur->next->left; }
        }
        leftmost = leftmost->left;
    }
    return root;
}

void test(const string& label, const vector<Node*>& all, const vector<int>& want) {
    connect(all.empty() ? nullptr : all[0]);
    vector<int> got;   // value of each node's next, 0 when next is nullptr
    for (Node* node : all) {
        got.push_back(node->next != nullptr ? node->next->val : 0);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    Node n1(1), n2(2), n3(3), n4(4), n5(5), n6(6), n7(7);
    n1.left = &n2;
    n1.right = &n3;
    n2.left = &n4;
    n2.right = &n5;
    n3.left = &n6;
    n3.right = &n7;
    test("perfect", {&n1, &n2, &n3, &n4, &n5, &n6, &n7}, {0, 3, 0, 5, 6, 7, 0});
    Node single(9);
    test("single", {&single}, {0});
    test("empty", {}, {});
}
