// dfs + hash-map

#include <vector>
#include <unordered_map>

class Solution {
private:
    std::unordered_map<Node*, Node*> visited;

    Node* dfs(Node* node) {
        if (!node) { return nullptr; }
        auto it = visited.find(node);
        if (it != visited.end()) { return it->second; }
        Node* copy = new Node(node->val);
        visited[node] = copy;

        copy->neighbors.reserve(node->neighbors.size());
        for (Node* nb : node->neighbors) {
            copy->neighbors.push_back(dfs(nb));
        }
        return copy;
    }

public:
    Node* cloneGraph(Node* node) {
        visited.clear();
        return dfs(node);
    }
};

// bfs + hash-map

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) { return nullptr; }

        std::unordered_map<Node*, Node*> visited;
        visited[node] = new Node(node->val);

        std::vector<Node*> q;
        q.push_back(node);
        int head = 0;

        while (head < static_cast<int>(q.size())) {
            Node* cur = q[head];
            head++;

            for (Node* nb : cur->neighbors) {
                if (visited.find(nb) == visited.end()) {
                    visited[nb] = new Node(nb->val);
                    q.push_back(nb);
                }
                visited[cur]->neighbors.push_back(visited[nb]);
            }
        }
        return visited[node];
    }
};
