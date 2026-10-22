// sort + scan, T: O(nlogn), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::sort

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        if (nums.empty()) { return 0; }
        std::sort(nums.begin(), nums.end());

        int maxLen = 1;
        int len = 1;
        for (int i = 1; i < static_cast<int>(nums.size()); i++) {
            if (nums[i] == nums[i - 1]) { continue; }
            len = (nums[i] == nums[i - 1] + 1) ? len + 1 : 1;
            maxLen = std::max(maxLen, len);
        }
        return maxLen;
    }
};

// hash-set, T: O(n) amortized, S: O(n)

#include <unordered_set>

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        std::unordered_set<int> visited;
        visited.reserve(nums.size());
        visited.max_load_factor(0.25f);
        visited.insert(nums.begin(), nums.end());

        int maxLen = 0;
        for (int v : visited) {
            if (visited.count(v - 1)) { continue; } // not a start

            int len = 1;
            while (visited.count(v + len)) { len++; }
            maxLen = std::max(maxLen, len);
        }
        return maxLen;
    }
};

// union-find, T: O(na(n)) ~= O(n), S: O(n)
// follow-up, only worth when streaming

#include <unordered_map>
#include <utility> // std::swap

class Solution {
private:
    std::unordered_map<int, int> id; // val -> dense-id
    std::vector<int> parent;
    std::vector<int> sz;
    int maxLen = 0;

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    int unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) { return; }
        if (sz[a] < sz[b]) { std::swap(a, b); }
        parent[b] = a;
        sz[a] += sz[b];
        maxLen = std::(maxLen, sz[a]);
    }

public:
    int add(int v) {
        auto [it, inserted] = id.try_emplace(v, static_cast<int>(parent.size()));
        if (!inserted) { return maxLen; } // duplicate

        parent.push_back(static_cast<int>(parent.size()));
        sz.push_back(1);
        maxLen = std::max(maxLen, 1);

        for (int d : {-1, 1}) { // both +- 1
            auto j = id.find(v + d);
            if (j != id.end()) { unite(it->second, j->second); }
        }
        return maxLen;
    }
};
