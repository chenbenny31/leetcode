// LC 559 Maximum Depth of an N-ary Tree: number of nodes on the longest root-to-leaf
// path (0 for an empty tree).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

struct Node {
    int val;
    vector<Node*> children;
    Node(int v) : val(v) {}
};

// DFS: a node's depth is 1 + the deepest child. T: O(n), S: O(h).
int maxDepth(Node* root) {
    if (root == nullptr) { return 0; }
    int deepest = 0;
    for (Node* child : root->children) { deepest = max(deepest, maxDepth(child)); }
    return deepest + 1;
}

void test(const string& label, Node* root, int want) {
    cout << label << (maxDepth(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    Node a(1), b(2), c(3), d(4), e(5);
    a.children = {&b, &c, &d};
    c.children = {&e};
    test("deep-middle", &a, 3);
    test("single", &b, 1);
    test("empty", nullptr, 0);
}

// follow-up: very deep tree -> BFS level by level and count the levels
