// LC 155 Min Stack (Medium) — stack of (value, min so far) pairs
// Design a stack with push, pop, top and getMin (smallest value currently stored), each
// in O(1).
// Clarify: pop / top / getMin on an empty stack (assume never called); duplicates of
//   the minimum; value range (matters only for the O(1) extra space trick).

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

// Core idea: store next to each value the minimum of everything at or below it; a pop
// removes the value and its minimum together, so the top pair always has the current
// minimum. T: O(1) per op, S: O(n).
class MinStack {
    // (value, min of the stack from the bottom up to this entry)
    vector<pair<int, int>> entries;

public:
    void push(int value) {
        int minSoFar = value;
        if (!entries.empty()) { minSoFar = min(value, entries.back().second); }
        entries.push_back({value, minSoFar});
    }

    void pop() {
        entries.pop_back();
    }

    int top() {
        return entries.back().first;
    }

    int getMin() {
        return entries.back().second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MinStack minStack;
    minStack.push(5);
    minStack.push(3);
    minStack.push(7);
    check(minStack.top() == 7 && minStack.getMin() == 3, "min-below-top");
    minStack.push(3);
    minStack.pop();
    check(minStack.getMin() == 3, "duplicate-min-survives-pop");
    minStack.pop();
    minStack.pop();
    check(minStack.top() == 5 && minStack.getMin() == 5, "min-restored-after-pop");
    return 0;
}

// follow-up (ideas only):
// - max stack with popMax (LC 716) -> two stacks give O(n) popMax (move values aside,
//   push them back); for O(log n) keep a list plus an ordered map value -> list iterators
// - O(1) extra space -> store 2 * x - min when x is a new minimum; a stored value below
//   min marks it, and the previous min is 2 * min - stored (use long long)
// - min queue -> monotonic deque of candidates: pop larger values from the back on push,
//   pop the front when it is the value leaving the queue
// - stack with increment of the bottom k (LC 1381) -> lazy add array: add[k - 1] += val,
//   apply it on pop and pass it down to add[top - 1]
// - queue with two stacks (LC 232) -> push to the in stack; when the out stack is empty
//   pour the in stack into it; O(1) amortized
// - frequency stack (LC 895) -> map value -> count and one stack per count; pop from the
//   stack of the highest count
