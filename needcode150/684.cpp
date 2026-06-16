// union-find, T: O(na(n)), S: O(n)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    std::vector<int> findRedundantConnection(std::vector<std::vector<int>>& edges) {
        int n = static_cast<int>(edges.size());
        std::vector<int> parent(n + 1);
        std::vector<int> rnk(n + 1, 0);
        for (int i = 0; i <= n ; i++) { parent[i] = i; }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int a, int b) -> bool {
            a = find(a); b = find(b);
            if (a == b) { return false; }
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            if (rnk[a] == rnk[b]) { rnk[a]++; }
            return true;
        };

        for (auto& e : edges) {
            if (!unite(e[0], e[1])) { return e; }
        }
        return {};
    }
};

// dfs, T: O(n2), S: O(n)

#include <vector>

class Solution {
public:
    std::vector<int> findRedundantConnection(std::vector<std::vector<int>>& edges) {
        int n = static_cast<int>(edges.size());
        std::vector<std::vector<int>> adj(n + 1);

        for (auto& e : edges) {
            std::vector<uint8_t> visited(n + 1, 0);
            if (dfs(adj, visited, e[0], e[1])) { return e; }
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        return {};
    }

private:
    bool dfs(std::vector<std::vector<int>>& adj,
             std::vector<uint8_t>& visited, int src, int dst) {
        if (src == dst) { return true; }
        visited[src] = 1;
        for (int nb : adj[src]) {
            if (!visited[nb] && dfs(adj, visited, nb, dst)) { return true; }
        }
        return false;
    }
};

// uf processes edges in order: first cycle is the answer
