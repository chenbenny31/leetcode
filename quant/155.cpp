// stack

#include <vector>
#include <stack>
#include <algorithm> // std::min

class MinStack {
private:
    std::stack<int>, std::vector<int>> stk;
    std::stack<int>, std::vector<int>> minStk;

public:
    MinStack() { }

    void push(int value) {
        stk.push(value);
        if (minStk.empty()) { minStk.push(value); }
        else { minStk.push(std::min(val, minStk.top()); }
    }

    void pop() {
        stk.pop();
        minStk.pop();
    }

    int top() {
        return stk.top();
    }

    int getMin() {
        return minStk.top();
    }
};

// one-stack: store val-curMin and curMin
