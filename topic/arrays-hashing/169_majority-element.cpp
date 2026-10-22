// Boyer-Moore Voting, T: O(n), S: O(1)
// every non-matching elem cancels one match; true majority holds more than half votes, never cancel

#include <vector>

class Solution {
public:
    int majorityElement(std::vector<int>& nums) {
        int cand = nums[0];
        int count = 0;

        for (int i = 0; i < static_cast<int>(nums.size()); i++) {
            if (count == 0) { cand = nums[i]; }
            count += (nums[i] == cand) ? 1 : -1;
        }
        return cand;
    }
};

// quick-select median, T: O(n), S: O(1)

#include <algorithm> // std::nth_element

class Solution {
public:
    int majorityElement(std::vector<int>& nums) {
        std::nth_element(nums.begin(), nums.begin() + nums.size() / 2, nums.end());
        return nums[nums.size() / 2];
    }
};

// per-bit reconstruction, T: O(n), S: O(1)
