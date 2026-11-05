// ByteDance / LC 133: return a deep copy of a connected undirected graph given one of
// its nodes (or nullptr). Each node has a val and a list of neighbors.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;

struct Node {
    int val;
    vector<Node*> neighbors;
};

// BFS; copy a node when first seen, then wire copied edges. T: O(V+E), S: O(V).
Node* cloneGraph(Node* node) {
    if (!node) { return nullptr; }
    // original node -> its clone
    unordered_map<Node*, Node*> copies;
    copies[node] = new Node{node->val, {}};
    // originals whose edges are not copied yet
    queue<Node*> q;
    q.push(node);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        for (Node* nxt : cur->neighbors) {
            // clone on first sight: map membership doubles as the visited set
            if (!copies.count(nxt)) {
                copies[nxt] = new Node{nxt->val, {}};
                q.push(nxt);
            }
            copies[cur]->neighbors.push_back(copies[nxt]);
        }
    }
    return copies[node];
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1 - 2
    // |   |
    // 4 - 3
    Node n1{1, {}};
    Node n2{2, {}};
    Node n3{3, {}};
    Node n4{4, {}};
    n1.neighbors = {&n2, &n4};
    n2.neighbors = {&n1, &n3};
    n3.neighbors = {&n2, &n4};
    n4.neighbors = {&n3, &n1};
    Node* c1 = cloneGraph(&n1);
    Node* c2 = c1->neighbors[0];
    Node* c4 = c1->neighbors[1];
    Node* c3 = c2->neighbors[1];
    bool fresh = c1 != &n1 && c2 != &n2 && c3 != &n3 && c4 != &n4;
    bool same = c1->val == 1 && c2->val == 2 && c3->val == 3 && c4->val == 4
                && c2->neighbors[0] == c1 && c3->neighbors[0] == c2
                && c3->neighbors[1] == c4 && c4->neighbors[0] == c3
                && c4->neighbors[1] == c1;
    check(fresh && same, "4-cycle");
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    Node solo{7, {}};
    Node* c = cloneGraph(&solo);
    check(c != &solo && c->val == 7 && c->neighbors.empty(), "single-node");
    delete c;
    check(cloneGraph(nullptr) == nullptr, "empty");
}

// follow-up: DFS version -> clone(node): return copies[node] if present; else create,
//   store in copies before recursing on neighbors (breaks cycles), O(V+E)
// follow-up: list with random pointer (LC 138) -> same old -> new map, or O(1) extra by
//   interleaving A->A'->B->B', set randoms, then split
// follow-up: disconnected graph -> BFS from every node not yet in copies
