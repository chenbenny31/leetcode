// Kahn's Algorithm (bfs topological sort), T: O(C), C = total chars across all words, S: O(1)

#include <vector>
#include <string>
#include <cstring> // std::memset
#include <cstdint> // uint8_t
#include <algorithm> // std::min

class Solution {
public:
    std::string alienOrder(std::vector<std::string>& words) {
        constexpr int R = 26;
        constexpr char BASE = 'a';

        uint8_t adj[R * R];
        std::memset(adj, 0, sizeof(adj));
        int indegree[R];
        std::memset(indegree, 0, sizeof(indegree));
        uint8_t present[R];
        std::memset(present, 0, sizeof(present));

        for (auto& w : words) {
            for (char c : w) { present[c - BASE] = 1; }
        }

        int n = static_cast<int>(words.size());
        for (int i = 0; i < n - 1; i++) {
            std::string& w1 = words[i];
            std::string& w2 = words[i+1];
            int len = std::min(static_cast<int>(w1.size()), static_cast<int>(w2.size()));
            
            bool found_diff = false;
            for (int j = 0; j < len; j++) {
                if (w1[j] != w2[j]) {
                    int u = w1[j] - BASE;
                    int v = w2[j] - BASE;
                    if (!adj[u * R + v]) {
                        adj[u * R + v] = 1;
                        indegree[v]++;
                    }
                    found_diff = true;
                    break;
                }
            }
            if (!found_diff && w1.size() > w2.size()) { return ""; }
        }

        std::vector<int> q;
        q.reserve(R);
        int head = 0;
        for (int c = 0; c < R; c++) {
            if (present[c] && indegree[c] == 0) { q.push_back(c); }
        }

        std::string result;
        result.reserve(R);

        while (head < static_cast<int>(q.size())) {
            int u = q[head];
            head++;
            result += static_cast<int>(u + BASE);
            for (int v = 0; v < R; v++) {
                if (adj[u * R + v]) {
                    indegree[v]--;
                    if (indegree[v] == 0) { q.push_back(v); }
                }
            }
        }

        int totalPresent = 0;
        for (int c = 0; c < R; c++) {
            if (present[c]) { totalPresent++; }
        }
        return static_cast<char>(result.size()) == totalPresent ? result : "";
    }
};

// dfs three-color cycle detection, T: O(C), S: O(1)

#include <vector>
#include <string>
#include <cstring> // std::memset
#include <cstdint> // uint8_t
#include <algorithm> // std::min, std::reverse

class Solution {
public:
    std::string alienOrder(std::vector<std::string>& words) {
        constexpr int R = 26;
        constexpr char BASE = 'a';

        uint8_t adj[R * R];
        std::memset(adj, 0, sizeof(adj));
        uint8_t present[R];
        std::memset(present, 0, sizeof(present));

        for (auto& w : words) {
            for (char c : w) { present[c - BASE] = 1; }
        }

        int n = static_cast<int>(words.size());
        for (int i = 0; i < n - 1; i++) {
            std::string& w1 = words[i];
            std::string& w2 = words[i+1];
            int len = std::min(static_cast<int>(w1.size()), static_cast<int>(w2.size()));

            bool found_diff = false;
            for (int j = 0; j < len; j++) {
                if (w1[j] != w2[j]) {
                    adj[(w1[j] - BASE) * R + (w2[j] - BASE)] = 1;
                    found_diff = true;
                    break;
                }
            }
            if (!found_diff && w1.size() > w2.size()) { return ""; }
        }

        uint8_t state[R]; // 0=white, 1=grey, 2=black
        std::memset(state, 0, sizeof(state));
        std::string result;
        result.reserve(R);

        for (int c = 0; c < R; c++) {
            if (present[c] && state[c] == 0) {
                if (dfs(c, adj, state, result)) { return ""; }
            }
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    bool dfs(int u, uint8_t* adj, uint8_t* state, std::string& result) {
        constexpr int R = 26;
        constexpr char BASE = 'a';
        state[u] = 1;
        for (int v = 0; v < R; v++) {
            if (!adj[u * R + v]) { continue; }
            if (state[v] == 1) { return true; }
            if (state[v] == 0 && dfs(v, adj, state, result)) { return true; }
        }
        state[u] = 2;
        result += static_cast<char>(u + BASE);
        return false;
    }
};
