// dfs + hash-map, T: O(V + E), S: O(V)

#include <unordered_map>
#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) { return nullptr; }
        std::unordered_map<Node*, Node*> cloned;
        cloned.reserve(128);
        cloned.max_load_factor(0.25f);
        return dfs(node, cloned);
    }

private:
    Node* dfs(Node* node, std::unordered_map<Node*, Node*>& cloned) {
        if (cloned.count(node)) { return cloned[node]; }
        Node* copy = new Node(node->val);
        cloned[node] = copy; // insert before recursing, handles cycles
        for (Node* nb : node->neighbors) {
            copy->neighbors.push_back(dfs(nb, cloned));
        }
        return copy;
    }
};

// bfs + hash-map, T: O(V + E), S: O(V)

#include <unordered_map>
#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) { return nullptr; }

        std::unordered_map<Node*, Node*> cloned;
        cloned.reserve(128);
        cloned.max_load_factor(0.25f);
        
        std::vector<Node*> q;
        q.reserve(128);
        int head = 0;

        cloned[node] = new Node(node->val);
        q.push_back(node);

        while (head < static_cast<int>(q.size())) {
            Node* curr = q[head];
            head++;
            for (Node* nb : curr->neighbors) {
                if (!cloned.count(nb)) {
                    cloned[nb] = new Node(nb->val);
                    q.push_back(nb);
                }
                cloned[curr]->neighbors.push_back(cloned[nb]);
            }
        }
        return cloned[node];
    }
};

// cloned[node] = copy before next: prevent cycle, A -> B -> A
// hash-map keyed by pointer: Node* as key
// dfs vs bfs: stack frames vs no stack overflow risk, bfs preferred for dense graphs or unknown depth
