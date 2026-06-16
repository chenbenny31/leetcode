// Kahn's algorithm bfs topological sort, T: O(V+E), S: O(V+E)

#include <vector>

class Solution {
public:
    bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
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
        return static_cast<int>(q.size()) == numCourses;
    }
};

// dfs cycle detection, T: O(V+E), S: O(V+E)

#include <vector>
#include <cstdint>

class Solution {
public:
    bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> adj(numCourses);
        for (auto& p : prerequisites) { adj[p[1]].push_back(p[0]); }
        
        std::vector<uint8_t> state(numCourses, 0); // 0=un-visited, 1=on-path, 2=visited

        for (int i = 0; i < numCourses; i++) {
            if (state[i] == 0 && hasCycle(adj, state, i)) { return false; }
        }
        return true;
    }

private:
    bool hasCycle(std::vector<std::vector<int>>& adj,
                  std::vector<uint8_t>& state, int node) {
        state[node] = 1; // mark on path
        for (int nb : adj[node]) {
            if (state[nb] == 1) { return true; }
            if (state[nb] == 0 && hasCycle(adj, state, nb)) { return true; }
        }
        state[node] = 2;
        return false;
    }
};

// Kahn's for dependency list: topological order = dependency list, any non-processed node is part of a cycle
// three state: on-path is used to detect cycle
// Kahn's vs dfs: iterative, no stack overflow, produce topological order, preferred for production
