// hash-set, T: O(n), S: O(n)

#include <vector>
#include <unordered_set>

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        std::unordered_set<int> seen(nums.begin(), nums.end());
        int best = 0;

        for (int x : seen) {
            if (seen.count(x - 1)) { continue; }
            int len = 1;
            while (seen.count(x + len)) { ++len; }
            best = std::max(best, len);
        }
        return best;
    }
};

// union-find, T: O(na(n)), S: O(n)

#include <vector>
#include <unordered_map>
#include <utility> // std::swap

class Solution {
private:
    std::unordered_map<int,int> parent, rnk, sz;

    int find(int x) {
        // if (parent[x] != x) { parent[x] = find(parent[x]); }
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) { return; }
        if (rnk[a] < rnk[b]) { std::swap(a, b); }
        parent[b] = a;
        sz[a] += sz[b];
        if (rnk[a] == rnk[b]) { ++rnk[a]; }
    }

public:
    int longestConsecutive(std::vector<int>& nums) {
        for (int x : nums) {
            parent[x] = x;
            rnk[x] = 0;
            sz[x] = 1;
        }
        for (int x : nums) {
            if (parent.count(x + 1)) { unite(x, x + 1); }
        }

        int best = 0;
        for (int x : nums) {
            best = std::max(best, sz[find(x)]);
        }
        return best;
    }
};

// hash-set iterate over seen not nums: nums has duplicate, seen avoids redundant seq head check
// UF overhead: three hash-map, for large n choose absl::flat_hash_map
// hash-set vs UF: UF enables online/streaming addition
// hash-set vs sorting: sort wins only when need O(1)
// bounded values: use flat-array over hash-map
// recursive vs iterative path-compression: iterative wins in avoid stack space and overflow risk
