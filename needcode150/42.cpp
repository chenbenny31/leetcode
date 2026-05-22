// two-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm>

class Solution {
public:
    int trap(std::vector<int>& height) {
        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int lmax = 0, rmax = 0;
        int res = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= lmax) { lmax = height[left]; }
                else { res += lmax - height[left]; }
                left++;
            } else {
                if (height[right] >= rmax) { rmax = height[right]; }
                else { res += rmax - height[right]; }
                right--;
            }
        }
        return res;
    }
};

// mono-stack, prefix-max: O(n) space
