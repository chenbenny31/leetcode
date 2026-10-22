// sliding-window + hash-set, T: O(n), S: O(min(n, k))

#include <vector>
#include <unordered_set>

class Solution {
public:
    bool containsNearbyDuplicate(std::vector<int>& nums, int k) {
        std::unordered_set<int> win; // vals at [i-k, i)
        win.reserve(k);

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (i > k) { win.erase(nums[i - k - 1]); }
            if (!win.insert(nums[i]).second) { return true; } // insert returns {iterator, bool}
        }
        return false;
    }
};

// hash-map (last-seen), T: O(n), S: O(n)

#include <unordered_map>

class Solution {
public:
    bool containsNearbyDuplicate(std::vector<int>& nums, int k) {
        std::unordered_map<int, int> lastSeen; // val -> lastIdx
        lastSeen.reserve(nums.size());

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            auto it = lastSeen.find(nums[i]);
            if (it != lastSeen.end() && i - it->second <= k) { return true; }
            lastSeen[nums[i]] = i;
        }
        return false;
    }
};
