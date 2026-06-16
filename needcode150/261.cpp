// union-find, T: O(na(n)), S: O(n)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    bool validTree(int n, std::vector<std::vector<int>>& edges) {
        if (static_cast<int>(edges.size()) != n - 1) { return false; }

        std::vector<int> parent(n);
        std::vector<int> rnk(n, 0);
        for (int i = 0; i < n; i++) { parent[i] = i; }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) { return false; } // detect cycle
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            if (rnk[a] == rnk[b]) { rnk[a]++; }
            return true;
        };

        for (auto& e : edges) {
            if (!unite(e[0], e[1])) { return false; }
        }
        return true;
    }
};

// dfs, T: O(V+E), S: O(V+E)

#include <vector>
#include <cstdint>
#include <algorithm> // std::all_of

class Solution {
public:
    bool validTree(int n, std::vector<std::vector<int>>& edges) {
        if (static_cast<int>(edges.size()) != n - 1) { return false; }

        std::vector<std::vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        std::vector<uint8_t> visited(n, 0);
        return dfs(adj, visited, 0, -1) &&
               std::all_of(visited.begin(), visited.end(), [](uint8_t v){ return v == 1; });
    }

private:
    bool dfs(std::vector<std::vector<int>>& adj,
             std::vector<uint8_t>& visited, int curr, int parent) {
        visited[curr] = 1;
        for (int nb : adj[curr]) {
            if (nb == parent) { continue; }
            if (visited[nb]) { return false; } // cycle
            if (!dfs(adj, visited, nb, curr)) { return false; }
        }
        return true;
    }
};

// edges.size() != n-1: early exit, valid tree has exactly n-1 edges
// undirected graph need parent tracking in dfs: parent helps skip the edge we comes from, prevent false cycle
// uf vs dfs: uf for better cache behavior and online, dfs for directed graph extension
