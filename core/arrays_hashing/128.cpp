// sort + scan, T: O(nlogn), S: O(1)
// hash-set, T: O(n) amortized, S: O(n)

// union-find, T: O(na(n)) ~= O(n), S: O(n)

#include <vector>
#include <unordered_map>
#include <utility> // std::swap
#include <algorithm> // std::max

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        if (n == 0) { return 0; }

        std::unordered_map<int, int> idx_map;
        idx_map.reserve(n);
        idx_map.max_load_factor(0.25f);

        std::vector<int> parent(n);
        std::vector<int> rnk(n, 1);
        std::vector<int> sz(n ,1);

        int uniq_cnt = 0;
        for (int i = 0; i < n; i++) {
            if (idx_map.find(nums[i]) == idx_map.end()) {
                idx_map[nums[i]] = uniq_cnt;
                parent[uniq_cnt] = uniq_cnt;
                uniq_cnt++;
            }
        }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        int best = 1;

        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) { return; }
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            sz[a] += sz[b];
            if (rnk[a] == rnk[b]) { rnk[a]++; }
            best = std::max(best, sz[a]);
        };

        for (auto& [val, idx] : idx_map) {
            auto it = idx_map.find(val + 1);
            if (it != idx_map.end()) {
                unite(idx, it->second);
            }
        }
        return best;
    }
};

// hash-map necessity: compress nums[i] from [-1e9, 1e9] down to [0, n)
// path-having make labmda style: without it find need reference its own type <- std::function, iter version never call itself
// uf wins hash-set: when need persist or extend components
