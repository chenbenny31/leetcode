// mono-stack

#include <vector>
#include <stack>
#include <algorithm> // std::max

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
        int n = static_cast<int>(heights.size());
        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf)); // idx with incre-h order

        int maxArea = 0;
        for (int i = 0; i <= n; i++) {
            int curH = (i == n) ? 0 : heights[i];
            while (!stk.empty() && curH < heights[stk.top()]) {
                int h = heights[stk.top()]; stk.pop();
                int w = stk.empty() ? i  : i - stk.top() - 1;
                maxArea = std::max(maxArea, h * w);
            }
            if (i < n) { stk.push(i); }
        }
        return maxArea;
    }
};
