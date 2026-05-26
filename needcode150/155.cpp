// aux min stack, flat array, T: O(1) all ops, S: O(n)

#include <algorithm> // std::min

class MinStack {
private:
    static constexpr int N = 30001;
    int stk[N];
    int min_stk[N];
    int top_ = -1;

public:
    MinStack() = default;

    void push(int val) {
        top_++;
        stk[top_] = val;
        min_stk[top_] = (top_ == 0) ? val : std::min(val, min_stk[top_ - 1]);
    }

    void pop() { top_--; }
    int top() { return stk[top_]; }
    int getMin() { return min_stk[top_]; }
};

// flat-array over std::stack: std::stack use std::deque, heap alloc per push; flat-array ~ 240KB fits in L2
// sync push: both array written at the same top
// std::min: branchless cmov on x86
// not single stack with pair {val, min}: arrays have better cache behavior
// tradeoff of delta encoding: store val - cur_min in single stack, complex logic on pop/top
