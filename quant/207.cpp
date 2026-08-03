// Kahn's

#include <vector>

class Solution {
public:
    bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        std::vector<std::vector<int>> adj(numCourses);
        std::vector<int> indegree(numCourses, 0);

        for (const std::vector<int>& p : prerequisites) {
            adj[p[1]].push_back(p[0]); // p[1] -> p[0]
            indegree[p[0]]++;
        }

        std::vector<int> q;
        int head = 0;
        q.reserve(numCourses);
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) { q.push_back(i); }
        }

        int done = 0;
        while (head < static_cast<int>(q.size())) {
            int cur = q[head];
            head++;
            done++;

            for (int nxt : adj[cur]) {
                indegree[nxt]--;
                if (indegree[nxt] == 0) { q.push_back(nxt); }
            }
        }
        return done == numCourses;
    }
};

// dfs + three-color marking

#include <vector>
#include <cstdint>

class Solution {
private:
    std::vector<std::vector<int>> adj;
    std::vector<uint8_t> state; // 0 = unvisited, 1 = on-path, 2 = explored

    bool hasCycle(int u) {
        state[u] = 1;
        for (int v : adj[u]) {
            if (state[v] == 1) { return true; }
            if (state[v] == 0 && hasCycle(v)) { return true; }
        }
        state[u] = 2;
        return false;
    }

public:
    bool canFinish(int numCourses, std::vector<std::vector<int>>& prerequisites) {
        adj.assign(numCourses, {});
        state.assign(numCourses, 0);

        for (const std::vector<int>& p : prerequisites) {
            adj[p[1]].push_back(p[0]);
        }

        for (int i = 0; i < numCourses; i++) {
            if (state[i] == 0 && hasCycle(i)) { return false; }
        }
        return true;
    }
};
