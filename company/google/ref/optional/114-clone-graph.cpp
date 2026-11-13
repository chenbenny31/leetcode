// 114 · Clone Graph — LC 133
// Mother problem: select/08-graphs/05-clone-graph.cpp
//
// Google reports: clone an undirected graph (1p3a)
//
// Clarify
// - input? → one node of a connected undirected graph, or nullptr
// - values unique? → don't rely on it; key the map by pointer
// - cycles / self-loops? → possible, the visited map handles them
// - return? → the clone of the given node
//
// Core idea
// - copies: original → clone; membership doubles as the visited set.
// - BFS: for each neighbor, clone it on first sight and enqueue it, then push
//   copies[nxt] onto copies[cur]'s neighbors.
// - T O(V + E), S O(V).
//
// Tests: square, single node, empty
//
// Follow-ups
// - DFS version → clone(node): return copies[node] if present; else create, store before
//   recursing on neighbors (breaks cycles)
// - list with random pointer (LC 138) → same old → new map
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// BFS; clone a node when first seen, then wire cloned edges. T: O(V+E), S: O(V).
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

// copy mirrors orig with all-new nodes; seen = original -> the copy it matched
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
    test("square", &n1);
    Node solo{7, {}};
    test("single-node", &solo);
    test("empty", nullptr);
}
