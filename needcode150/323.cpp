// union-find, T: O(na(n)), S: O(n)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    int countComponents(int n, std::vector<std::vector<int>>& edges) {
        std::vector<int> parent(n);
        std::vector<int> rnk(n, 0);
        for (int i = 0; i < n; i++) { parent[i] = i; }
        int count = n;

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) { return; }
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            rnk[a]++;
            if (rnk[a] == rnk[b]) { rnk[a]++; }
            count--;
        };

        for (auto& e : edges) { unite(e[0], e[1]); }
        return count;
    }
};

// dfs, T: O(V+E), S: O(V+E)

#include <vector>
#include <cstdint>

class Solution {
public:
    int countComponents(int n, std::vector<std::vector<int>>& edges) {
        std::vector<std::vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        std::vector<uint8_t> visited(n, 0);
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(adj, visited, i);
                count++;
            }
        }
        return count;
    }

private:
    void dfs(std::vector<std::vector<int>>& adj,
             std::vector<uint8_t>& visited, int node) {
        visited[node] = 1;
        for (int nb : adj[node]) {
            if (!visited[nb]) { dfs(adj, visited, nb); }
        }
    }
};

// count=n initailized, count-- on union: final count=number of distinct components
// uf vs dfs: uf supports daynamic connectivity and cache-friendly
