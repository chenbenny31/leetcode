// single-pass coverage check, T: O(n), S: O(1)

#include <vector>

class Solution {
public:
    bool mergeTriplets(std::vector<std::vector<int>>& triplets,
                       std::vector<int>& target) {
        bool hit_x = false;
        bool hit_y = false;
        bool hit_z = false;

        for (auto& t : triplets) {
            if (t[0] > target[0] || t[1] > target[1] || t[2] > target[2]) { continue; }
            if (t[0] == target[0]) { hit_x = true; }
            if (t[1] == target[1]) { hit_y = true; }
            if (t[2] == target[2]) { hit_z = true; }
            if (hit_x && hit_y && hit_z) { return true; }
        }
        return hit_x && hit_y && hit_z;
    }
};
