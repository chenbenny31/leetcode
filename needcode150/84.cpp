// monotonic-stack, T: O(n), S: O(n)

#include <vector>
#include <stack>
#include <algorithm> // std::max

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
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
};

// dp, T: O(n), S: O(n)

#include <vector>
#include <algorithm> // std::max
#include <stack>

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
        int n = static_cast<int>(heights.size());
        std::vector<int> left(n), right(n);
        std::vector<int> stk;
        stk.reserve(n);
        int top = -1;

        for (int i = 0; i < n; i++) {
            while (top != -1 && heights[stk[top]] >= heights[i]) { top--; }
            left[i] = (top == -1) ? -1 : stk[top];
            top++;
            stk[top] = i;
        }

        stk.clear();
        top = -1;

        for (int i = n - 1; i >= 0; i--) {
            while (top != -1 && heights[stk[top]] >= heights[i]) { top--; }
            right[i] = (top == -1) ? n : stk[top];
            top++;
            stk[top] = i;
        }

        int best = 0;
        for (int i = 0; i < n; i++) {
            best = std::max(best, heights[i] * (right[i] - left[i] - 1));
        }
        return best;
    }
};

// monotonic-stack: meet lower bar -> trigger one time area comput and comp
// sentinel: i = 0 and i = n
// incre stack not decre: meet higher bar keep accum, meet lower bar triggeiir
// monotonic-stack vs dp: stk is strictly better
