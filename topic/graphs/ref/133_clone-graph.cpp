// LC 133 Clone Graph: given a node of a connected undirected graph, return a deep copy
// of the whole graph. Node = value + list of neighbors.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;
using std::unordered_set;

struct Node {
    int val;
    vector<Node*> neighbors;
    Node(int v) : val(v) {}
};

// BFS that creates each clone the first time its original is seen, then wires edges.
// T: O(V+E), S: O(V).
Node* cloneGraph(Node* node) {
    if (node == nullptr) { return nullptr; }
    unordered_map<Node*, Node*> copy;   // original -> its clone; also visited
    queue<Node*> q;                     // originals whose edges are not copied yet
    copy[node] = new Node(node->val);
    q.push(node);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        for (Node* nxt : cur->neighbors) {
            if (copy.count(nxt) == 0) {
                copy[nxt] = new Node(nxt->val);
                q.push(nxt);
            }
            copy[cur]->neighbors.push_back(copy[nxt]);
        }
    }
    return copy[node];
}

vector<Node*> bfsOrder(Node* root) {
    vector<Node*> order;
    if (root == nullptr) { return order; }
    unordered_set<Node*> seen = {root};
    order.push_back(root);
    for (int i = 0; i < (int)order.size(); i++) {
        for (Node* nxt : order[i]->neighbors) {
            if (seen.insert(nxt).second) { order.push_back(nxt); }
        }
    }
    return order;
}

// same values and edges, but no node shared with the original
void test(const string& label, Node* root) {
    Node* copy = cloneGraph(root);
    vector<Node*> a = bfsOrder(root), b = bfsOrder(copy);
    int n = a.size();
    bool ok = n == (int)b.size();
    for (int i = 0; ok && i < n; i++) {
        ok = a[i] != b[i] && a[i]->val == b[i]->val &&
             a[i]->neighbors.size() == b[i]->neighbors.size();
        for (int k = 0; ok && k < (int)a[i]->neighbors.size(); k++) {
            ok = a[i]->neighbors[k]->val == b[i]->neighbors[k]->val;
        }
    }
    for (Node* x : b) { delete x; }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node a(1), b(2), c(3), d(4);
    a.neighbors = {&b, &c};
    b.neighbors = {&a, &c};
    c.neighbors = {&a, &b, &d};
    d.neighbors = {&c};
    Node solo(7);
    test("cycle-with-tail", &a);
    test("single", &solo);
    test("null", nullptr);
}
