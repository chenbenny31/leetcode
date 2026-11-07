// LC 559 Maximum Depth of an N-ary Tree: number of nodes on the longest root-to-leaf
// path (0 for an empty tree).

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

struct Node {
    int val;
    vector<Node*> children;
    Node(int v) : val(v) {}
};

// BFS by levels; count the levels. T: O(n), S: O(width).
int maxDepth(Node* root) {
    if (root == nullptr) { return 0; }
    queue<Node*> q;
    q.push(root);
    int depth = 0;
    while (!q.empty()) {
        int size = q.size();
        for (int k = 0; k < size; k++) {
            Node* cur = q.front();
            q.pop();
            for (Node* nxt : cur->children) { q.push(nxt); }
        }
        depth++;
    }
    return depth;
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
