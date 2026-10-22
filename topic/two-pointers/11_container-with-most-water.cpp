// two-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::min, std::max

class Solution {
public:
    int maxArea(std::vector<int>& height) {
        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int maxArea = 0;

        while (left < right) {
            int w = right - left;
            int h = std::min(height[left], height[right]);
            maxArea = std::max(maxArea, h * w);

            if (height[left] <= height[right]) {
                left++;
            } else {
                right--;
            }
        }
        return maxArea;
    }
};
