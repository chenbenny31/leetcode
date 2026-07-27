// mono-stack

#include <vector>
#include <stack>
#include <algorithm> // std::max

class Solution {
private:
    int largestRectangleArea(std::vector<int>& heights) {
        const int n = static_cast<int>(heights.size());
        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf)); // idx with incre h

        int maxArea = 0;
        for (int i = 0; i <= n; i++) {
            int curH = (i == n) ? 0 : heights[i];
            while (!stk.empty() && curH < heights[stk.top()]) {
                int h = heights[stk.top()]; stk.pop();
                int w = stk.empty() ? i : i - stk.top() - 1;
                maxArea = std::max(maxArea, h * w);
            }
            if (i < n) { stk.push(i); }
        }
        return maxArea;
    }

public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) { return 0; }
        const int m = static_cast<int>(matrix.size());
        const int n = static_cast<int>(matrix[0].size());
        std::vector<int> heights(n, 0);

        int maxArea = 0;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (matrix[r][c] == '1') { heights[c]++; }
                else { heights[c] = 0; }
            }
            maxArea = std::max(maxArea, largestRectangleArea(heights));
        }
        return maxArea;
    }
};

// dp

#include <vector>
#include <algorithm> // std::max
#include <climits> // INT_MAX

class Solution {
public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) { return 0; }
        const int m = static_cast<int>(matrix.size());
        const int n = static_cast<int>(matrix[0].size());

        std::vector<int> height(n, 0); // cur row with consec num of 1's in prev row (depth here)
        std::vector<int> left(n, 0); // leftmost col cover consec 1's until left[i]
        std::vector<int> right(n, n); // rightmost col cover consec 1's until right[i]
        int maxArea = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                height[c] = (matrix[r][c] == '1') ? height[c] + 1 : 0;
            }

            int curLeft = 0;
            for (int c = 0; c < n; c++) {
                if (matrix[r][c] == '1') { left[c] = std::max(left[c], curLeft); }
                else { left[c] = 0; curLeft = c + 1; }
            }

            int curRight = n;
            for (int c = n - 1; c >= 0; c--) {
                if (matrix[r][c] == '1') { right[c] = std::min(right[c], curRight); }
                else { right[c] = n; curRight = c; }
            }

            for (int c = 0; c < n; c++) {
                if (matrix[r][c] == '1') { maxArea = std::max(maxArea, height[c] * (right[c] - left[c])); }
            }
        }
        return maxArea;
    }
};
