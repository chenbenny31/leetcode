// dp with pre-compute left/right max arrays, T: O(n), S: O(n)

// monotonic-stack, T: O(n), S: O(n)

#include <vector>
#include <stack>
#include <algorithm> // std::min

class Solution {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());

        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf));

        int water = 0;
        for (int i = 0; i < n; i++) {
            while (!stk.empty() && height[i] > height[stk.top()]) {
                int bottom = stk.top(); stk.pop();
                if (stk.empty()) { break; } // no left wall already
                int left = stk.top();
                int wt = i - left - 1;
                int h = std::min(height[left], height[i]) - height[bottom];
                water += wt * h;
            }
            stk.push(i);
        }
        return water;
    }
};

// two-pointers, T: O(n), S: O(1)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int trap(std::vector<int>& height) {
        int left = 0;
        int right = static_cast<int>(height.size()) - 1;
        int leftMax = 0;
        int rightMax = 0;
        int water = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                leftMax = std::max(leftMax, height[left]);
                water += leftMax - height[left];
                left++;
            } else {
                rightMax = std::max(rightMax, height[right]);
                water += rightMax - height[right];
                right--;
            }
        }
        return water;
    }
};

// two-pointer correctness: each comp determines the lower side water with one step shrink, since leftMax <= max(rightMax, height[right])
// monotonic-stack: computes a horizontal rectangle when a higher bar occurs -> repeatly compute water with bottom until left is higher
