// monotonic-stack + running-min, T: O(n), S: O(n)

#include <vector>
#include <climits> // INT_MIN

class Solution {
public:
    bool find132pattern(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> stk;
        stk.reserve(n);
        int top = -1;
        int third = INT_MIN; // '2' in 132, largest popped val

        for (int i = n - 1; i >= 0; i--) {
            if (nums[i] < third) { return true; }
            while (top != -1 && nums[i] > stk[top]) {
                third = stk[top];
                top--;
            }
            top++;
            stk[top] = nums[i];
        }
        return false;
    }
};

// scan right to left: need i < j < k with nums[i] < nums[k] < nums[j]
// third is the largest popped val: monotonic decre stk
// cache right-to-left seq scan: prefetcher handles reverse stride
