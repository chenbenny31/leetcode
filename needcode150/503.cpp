// monotonic-stack + circular-array, T: O(n), S: O(n)

#include <vector>
#include <stack>
#include <climits> // INT_MIN

class Solution {
public:
    std::vector<int> nextGreaterElements(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> out(n, -1);

        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf));

        for (int i = 0; i < 2 * n; i++) {
            int idx = i % n;
            while (!stk.empty() && nums[idx] > nums[stk.top()]) {
                out[stk.top()] = nums[idx];
                stk.pop();
            }
            if (i < n) { stk.push(idx); } // push indices only in first pass
        }
        return out;
    }
};

// store indices not vals: circular array, must store indices to correctly write out[stk.top()]
// if (i < n) stk.push(idx): only push in first pass, second pass resolves rem entries merely
// 2n suffices: each element need at most one full circular scan: n + n
