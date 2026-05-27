// monotinic-stack per row, T: O(mn), S: O(n)

#include <vector>
#include <algorithm> // std::max
#include <stack>

class Solution {
private:
    int maxPerRow(std::vector<int>& heights) {
        int n = static_cast<int>(heights.size());

        std::vector<int> buf;
        buf.reserve(n + 1);
        std::stack<int, std::vector<int>> stk(std::move(buf));

        int best = 0;
        for (int i = 0; i <= n; i++) {
            int cur_h = (i == n) ? 0 : heights[i];
            while (!stk.empty() && cur_h < heights[stk.top()]) {
                int h = heights[stk.top()]; stk.pop();
                int w = stk.empty() ? i : i - stk.top() - 1;
                best = std::max(best, h * w);
            }
            stk.push(i);
        }
        return best;
    }

public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) { return 0; }

        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        std::vector<int> heights(n, 0);
        int best = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                heights[c] = (matrix[r][c] == '1') ? heights[c] + 1 : 0;
            }
            best = std::max(best, maxPerRow(heights));
        }
        return best;
    }
};

// dp, T: O(mn), S: O(n)

#include <vector>
#include <algorithm>
#include <climits> // INT_MAX

class Solution {
public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) { return 0; }

        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());

        std::vector<int> height(n, 0);
        std::vector<int> left(n, 0);
        std::vector<int> right(n, n);
        int best = 0;

        for (int r = 0; r < m; r++) {
            // update height
            for (int c = 0; c < n; ++c) {
                height[c] = (matrix[r][c] == '1') ? height[c] + 1: 0;
            }

            // update left boundary
            int cur_left = 0;
            for (int c = 0; c < n; ++c) {
                if (matrix[r][c] == '1') { left[c] = std::max(left[c], cur_left); }
                else { left[c] = 0; cur_left = c + 1; }
            }

            // update right boundary
            int cur_right = n;
            for (int c = n - 1; c >= 0; --c) {
                if (matrix[r][c] == '1') { right[c] = std::min(right[c], cur_right); }
                else { right[c] = n; cur_right = c; }
            }

            // compute max area
            for (int c = 0; c < n; c++) {
                if (matrix[r][c] == '1') { best = std::max(best, height[c] * (right[c] - left[c])); }
            }
        }
        return best;
    }
};

// height update rule: accum consecutive '1' vertically, '0' resets streak
// reuse of single heights[]: no 2d storage cost
// dp vs monotonic-stack: dp prefer for always consecutive access, stack has random acess heights[stack.top()]
