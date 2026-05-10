// monotonic-stack + circular-array, T: O(n), S: O(n)

#include <vector>
#include <stack>

class Solution {
public:
    std::vector<int> nextGreaterElements(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> out(n, -1);
        std::stack<int> stk; // indices of monotonically decreasing values

        for (int i = 0; i < 2 * n; ++i) {
            int idx = i % n;
            while (!stk.empty() && nums[idx] > nums[stk.top()]) {
                out[stk.top()] = nums[idx];
                stk.pop();
            }
            if (i < n) { stk.push(idx); }
        }
        return out;
    }
};

// process 2n: each element needs ont full circular scan
// if (i < n): only push indices in the first pass
// i % n: avoid two pass with O(1) division on hot path
