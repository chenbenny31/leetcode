// monotonic-stack, T: O(n), S: O(n)

#include <vector>

class Solution {
public:
    std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
        int n = static_cast<int>(temperatures.size());
        std::vector<int> out(n, 0);

        std::vector<int> stk;
        stk.reserve(n);
        int top = -1;

        for (int i = 0; i < n; i++) {
            while (top != -1 && temperatures[i] > temperatures[stk[top]]) {
                int idx = stk[top];
                top--;
                out[idx] = i - idx;
            }
            top++;
            stk[top] = i;
        }
        return out;
    }
};

// amortized O(1): each idx push and pop once
// std::vector vs int[MAX_N]: alloc with working set size for better cache perf
