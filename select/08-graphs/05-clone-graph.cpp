// LC 133 Clone Graph (Medium) — BFS with an original-to-clone map
// deep copy a connected undirected graph given one node (val + neighbors list)
//
// clarify:
// null input -> null
// connected? -> yes; else BFS from every node (follow-up)
// self-loops / parallel edges -> copied as is
// values unique? -> not needed, key by pointer
// who frees the copy -> caller

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

// core idea: BFS over originals; clone a node on first sight, copy each edge via the map
// T: O(V + E), S: O(V)
Node* cloneGraph(Node* node) {
    if (!node) { return nullptr; }
    // original -> its clone; membership = visited
    unordered_map<Node*, Node*> copies;
    copies[node] = new Node{node->val, {}};
    // originals whose edges are not copied yet
    queue<Node*> q;
    q.push(node);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        for (Node* nxt : cur->neighbors) {
            if (!copies.count(nxt)) {
                copies[nxt] = new Node{nxt->val, {}};
                q.push(nxt);
            }
            copies[cur]->neighbors.push_back(copies[nxt]);
        }
    }
    return copies[node];
}

// seen = original -> copy it was matched with
bool sameGraph(Node* orig, Node* copy, unordered_map<Node*, Node*>& seen) {
    if (seen.count(orig)) { return seen[orig] == copy; }
    if (orig == copy || orig->val != copy->val) { return false; }
    if (orig->neighbors.size() != copy->neighbors.size()) { return false; }
    seen[orig] = copy;
    for (int i = 0; i < (int)orig->neighbors.size(); i++) {
        if (!sameGraph(orig->neighbors[i], copy->neighbors[i], seen)) { return false; }
    }
    return true;
}

void test(const string& label, Node* node) {
    Node* copy = cloneGraph(node);
    unordered_map<Node*, Node*> seen;
    bool ok = node ? sameGraph(node, copy, seen) : copy == nullptr;
    for (auto& [orig, clone] : seen) { delete clone; }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node n1{1, {}};
    Node n2{2, {}};
    Node n3{3, {}};
    Node n4{4, {}};
    n1.neighbors = {&n2, &n4};
    n2.neighbors = {&n1, &n3};
    n3.neighbors = {&n2, &n4};
    n4.neighbors = {&n3, &n1};
    test("square", &n1);
    Node solo{7, {}};
    test("single-node", &solo);
    test("empty", nullptr);
    return 0;
}

// follow-up:
// DFS version -> clone(node): return stored clone, else create + store BEFORE recursing
// copy list w/ random pointer (LC 138) -> same map: pass 1 clones, pass 2 wires
// disconnected graph as node list -> same BFS from every node w/o a clone yet
