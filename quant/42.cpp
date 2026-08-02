// two-pointers

#include <vector>
#include <algorithm>

class Solution {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());
        int left = 0;
        int right = n - 1;
        int leftMax = 0;
        int rightMax = 0;
        int res = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                leftMax = std::max(leftMax, height[left]);
                res += leftMax - height[left];
                left++;
            } else {
                rightMax = std::max(rightMax, height[right]);
                res += rightMax - height[right];
                right--;
            }
        }
        return res;
    }
};

// mono-stack

#include <stack>

class Solution {
public:
    int trap(std::vector<int>& height) {
        const int n = static_cast<int>(height.size());

        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf)); // stores idx with decre h
        int res = 0;

        for (int i = 0; i < n; i++) {
            while (!stk.empty() && height[i] > height[stk.top()]) {
                int bottom = stk.top(); stk.pop();
                if (stk.empty()) { break; } // no left wall
                int left = stk.top();
                int w = i - left - 1; // [left, i] with bottom
                int h = std::min(height[left], height[i]) - height[bottom];
                res += w * h;
            }
            stk.push(i);
        }
        return res;
    }
};
