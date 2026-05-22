// two-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::min

class Solution {
public:
    int maxArea(std::vector<int>& height) {
        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int best = 0;

        while (left < right) {
            int curr = std::min(height[left], height[right]) * (right - left);
            best = std::max(best, curr);
            if (height[left] < height[right]) { left++; }
            else { right--; }
        }
        return best;
    }
};

// moving shorter ptr correctness: width shrinks, but min height might incre
// branchless ptr adv: compiles to cmov
left += (height[left] < height[right]);
right -= (height[left] >= height[right]);
// prove correctness: by contradiction

