// Hierholzer's algorithm, dfs with post-order collection, T: O(nlogn), S: O(n)

#include <vector>
#include <string>
#include <map>
#include <algorithm> // std::sort, std::reverse
#include <functional> // std::greater

class Solution {
public:
    std::vector<std::string> findItinerary(std::vector<std::vector<std::string>>& tickets) {
        std::map<std::string, std::vector<std::string>> adj;
        for (auto& t : tickets) { adj[t[0]].push_back(t[1]); }
        for (auto& [src, dsts] : adj) {
            std::sort(dsts.begin(), dsts.end(), std::greater<std::string>());
        }

        std::vector<std::string> out;
        out.reserve(tickets.size() + 1);
        dfs("JFK", adj, out);
        std::reverse(out.begin(), out.end());
        return out;
    }

private:
    void dfs(const std::string& src,
             std::map<std::string, std::vector<std::string>>& adj,
             std::vector<std::string>& out) {
        auto it = adj.find(src);
        if (it != adj.end()) {
            auto& dsts = it->second;
            while (!dsts.empty()) {
                std::string dst = dsts.back();
                dsts.pop_back();
                dfs(dst, adj, out);
            }
        }
        return out.push_back(src);
    }
};

// Hierholzer's post-order collection: node added to out after all its outgoing edges explored
// std::greater<std::string>: lexicographically largest at back, pop_back takes smallest lexicographical order
// std::map not std::unordered_map: std::map sort keys, consistent traversal order
// post order: explore all edgest first, add to out when exhuasted, guarantees all ticket used
