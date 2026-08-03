// Kahn's

#include <vector>

class Solution {
public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> adj(numCourses);
        std::vector<int> indegree(numCourses, 0);

        for (const std::vector<int>& p : prerequisites) {
            adj[p[1]].push_back(p[0]); // p[1] -> p[0]
            indegree[p[0]]++;
        }

        std::vector<int> order;
        order.reserve(numCourses);
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) { order.push_back(i); }
        }

        int head = 0;
        while (head < static_cast<int>(order.size())) {
            int cur = order[head];
            head++;
            for (int nxt : adj[cur]) {
                indegree[nxt]--;
                if (indegree[nxt] == 0) { order.push_back(nxt); }
            }
        }

        if (static_cast<int>(order.size()) != numCourses) { return {}; }
        return order;
    }
};

// dfs post-order + reverse

#include <vector>
#include <cstdint>
#include <algorithm> // std::reverse

class Solution {
private:
    std::vector<std::vector<int>> adj;
    std::vector<std::uint8_t> state; // 0 = unvisited, 1 = on-path, 2 = explored
    std::vector<int> order;

    bool hasCycle(int u) {
        state[u] = 1;
        for (int v : adj[u]) {
            if (state[v] == 1) { return true; }
            if (state[v] == 0 && hasCycle(v)) { return true; }
        }
        state[u] = 2;
        order.push_back(u); // post-order: push after all descendants
        return false;
    }

public:
    std::vector<int> findOrder(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        adj.assign(numCourses, {});
        state.assign(numCourses, 0);
        order.clear();
        order.reserve(numCourses);

        for (const std::vector<int>& p : prerequisites) {
            adj[p[1]].push_back(p[0]);
        }

        for (int i = 0; i < numCourses; i++) {
            if (state[i] == 0 && hasCycle(i)) { return {}; }
        }

        std::reverse(order.begin(), order.end());
        return order;
    }
};
