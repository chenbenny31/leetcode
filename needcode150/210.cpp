// Kahn's algorithm, T: O(V+E), S: O(V+E)

#include <vector>

class Solution {
public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> adj(numCourses);
        std::vector<int> indeg(numCourses, 0);
        for (auto& p : prerequisites) {
            adj[p[1]].push_back(p[0]);
            indeg[p[0]]++;
        }

        std::vector<int> q;
        q.reserve(numCourses);
        int head = 0;

        for (int i = 0; i < numCourses; i++) {
            if (indeg[i] == 0) { q.push_back(i); }
        }
        
        while (head < static_cast<int>(q.size())) {
            int curr = q[head];
            head++;
            for (int nb : adj[curr]) {
                indeg[nb]--;
                if (indeg[nb] == 0) { q.push_back(nb); }
            }
        }

        if (static_cast<int>(q.size()) != numCourses) { return {}; }
        return q;
    }
};

// dfs postorder, T: O(V+E), S: O(V+E)

#include <vector>
#include <cstdint>
#include <algorithm>

class Solution {
public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> adj(numCourses);
        for (auto& p : prerequisites) { adj[p[1]].push_back(p[0]); }

        std::vector<uint8_t> state(numCourses, 0);
        std::vector<int> order;
        order.reserve(numCourses);

        for (int i = 0; i < numCourses; i++) {
            if (state[i] == 0 && !dfs(adj, state, order, i)) { return {}; }
        }
        std::reverse(order.begin(), order.end());
        return order;
    }

private:
    bool dfs(std::vector<std::vector<int>>& adj,
             std::vector<uint8_t>& state,
             std::vector<int>& order, int node) {
        state[node] = 1;
        for (int nb : adj[node]) {
            if (state[nb] == 1) { return false; } // detect cycle
            if (state[nb] == 0 && !dfs(adj, state, order, nb)) { return false; }
        }
        state[node] = 2;
        order.push_back(node);
        return true;
    }
};

// dfs postorder -> topological order: node pushed after all descendants, reverse to give topological order
