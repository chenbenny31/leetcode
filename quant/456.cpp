// mono-stack

#include <vector>
#include <stack>
#include <climits> // INT_MIN
#include <algorithm> // std::max

class Solution {
public:
    bool find132pattern(vector<int>& nums) {
        const int n = static_cast<int>(nums.size()); 
        std::vector<int> buf;
        buf.reserve(n);
        std::stack<int, std::vector<int>> stk(std::move(buf));

        int third = INT_MIN; // the 2 in 132
        for (int i = n - 1; i >= 0; i--) {
            if (nums[i] < third) { return true; } // find a valid 1
            while (!stk.empty() && stk.top() < nums[i]) { // find a greater 3
                third = std::max(third, stk.top());
                stk.pop();
            }
            stk.push(nums[i]);
        }
        return false;
    }
};
