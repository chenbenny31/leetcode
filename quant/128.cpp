// hash-set

#include <unordered_set>
#include <algorithm>

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        std::unordered_set<int> seen(nums.begin(), nums.end());
        int maxLen = 0;

        for (int x : seen) {
            if (seen.count(x - 1)) { continue; }

            int len = 1;
            while (seen.count(x + len)) { len++; }
            maxLen = std::max(maxLen, len);
        }
        return maxLen;
    }
};

// union-find

#include <unordered_map>

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        if (n == 0) { return 0; }
        std::unordered_map<int, int> idxMap; // nums[i]: i, translace unbounded range into [0, n)
        idxMap.reserve(n);
        idxMap.max_load_factor(0.25f);

        std::vector<int> parent(n);
        std::vector<int> sz(n, 1);

        int uniqCnt = 0;
        for (int i = 0; i < n; i++) {
            auto [it, inserted] = idxMap.try_emplace(nums[i], uniqCnt); // .insert({nums[i], uniqCnt});
            if (inserted) {
                parent[uniqCnt] = uniqCnt;
                uniqCnt++;
            }
        }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        int maxLen = 1;

        auto unite = [&](int a, int b) {
            a = find(a);
            b = find(b);
            if (a == b) { return; }
            if (sz[a] < sz[b]) { std::swap(a, b); }
            parent[b] = a;
            sz[a] += sz[b];
            maxLen = std::max(maxLen, sz[a]);
        };

        for (auto& [val, idx] : idxMap) {
            auto it = idxMap.find(val + 1);
            if (it != idxMap.end()) { unite(idx, it->second); }
        }
        return maxLen;
    }
};
