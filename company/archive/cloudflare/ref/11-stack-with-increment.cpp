// LC 1381 Design a Stack With Increment Operation (Medium). Pattern: lazy increments.
// CustomStack(maxSize). push(x) does nothing when the stack is full; pop() returns the
// top or -1 when empty; increment(k, val) adds val to the bottom k elements (all of
// them if there are fewer than k).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::min;
using std::string;
using std::vector;

// Straightforward: a vector as the stack; increment loops over the bottom k.
// T: push / pop O(1), increment O(k), S: O(maxSize).
class CustomStack {
    // stack values, bottom at index 0
    vector<int> values;
    int maxSize;

public:
    CustomStack(int capacity) : maxSize(capacity) {}

    void push(int x) {
        if ((int)values.size() == maxSize) { return; }
        values.push_back(x);
    }

    int pop() {
        if (values.empty()) { return -1; }
        int top = values.back();
        values.pop_back();
        return top;
    }

    void increment(int k, int val) {
        int count = min(k, (int)values.size());
        for (int i = 0; i < count; i++) {
            values[i] += val;
        }
    }
};

// Improved: record the add once at the highest affected index; when that element is
// popped, it takes the add and passes it down to the element below.
// T: O(1) per op, S: O(maxSize).
class LazyStack {
    // stack values without pending adds, bottom at index 0
    vector<int> values;
    // inc[i] = pending add for every element at index 0..i
    vector<int> inc;
    int maxSize;

public:
    LazyStack(int capacity) : maxSize(capacity) {}

    void push(int x) {
        if ((int)values.size() == maxSize) { return; }
        values.push_back(x);
        inc.push_back(0);
    }

    int pop() {
        if (values.empty()) { return -1; }
        int top = (int)values.size() - 1;
        int result = values[top] + inc[top];
        // the element below inherits the pending add
        if (top > 0) { inc[top - 1] += inc[top]; }
        values.pop_back();
        inc.pop_back();
        return result;
    }

    void increment(int k, int val) {
        // highest index the add reaches
        int last = min(k, (int)values.size()) - 1;
        if (last >= 0) { inc[last] += val; }
    }
};

// op codes: {PUSH, x}, {POP}, {INC, k, val}
const int PUSH = 0;
const int POP = 1;
const int INC = 2;

// returns what each pop returned
vector<int> runNaive(int maxSize, const vector<vector<int>>& ops) {
    CustomStack stack(maxSize);
    vector<int> pops;
    for (const vector<int>& op : ops) {
        if (op[0] == PUSH) { stack.push(op[1]); }
        else if (op[0] == POP) { pops.push_back(stack.pop()); }
        else { stack.increment(op[1], op[2]); }
    }
    return pops;
}

vector<int> runLazy(int maxSize, const vector<vector<int>>& ops) {
    LazyStack stack(maxSize);
    vector<int> pops;
    for (const vector<int>& op : ops) {
        if (op[0] == PUSH) { stack.push(op[1]); }
        else if (op[0] == POP) { pops.push_back(stack.pop()); }
        else { stack.increment(op[1], op[2]); }
    }
    return pops;
}

void test(const string& label, int maxSize, const vector<vector<int>>& ops,
          const vector<int>& want) {
    bool ok = runNaive(maxSize, ops) == want && runLazy(maxSize, ops) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("full-and-empty", 2, {{PUSH, 1}, {PUSH, 2}, {PUSH, 3}, {POP}, {POP}, {POP}},
         {2, 1, -1});
    test("inc-bottom-k", 3, {{PUSH, 1}, {PUSH, 2}, {PUSH, 3}, {INC, 2, 10}, {POP},
                             {POP}, {POP}},
         {3, 12, 11});
    test("inc-past-size", 3, {{PUSH, 1}, {PUSH, 2}, {INC, 5, 100}, {POP}, {PUSH, 7},
                              {POP}, {POP}},
         {102, 7, 101});
}

// follow-up: why pass the add down on pop -> inc[i] covers 0..i; after index i leaves,
//   elements 0..i-1 still owe it, so it moves to inc[i-1].
// follow-up: increment on a range [l, r] -> difference trick: inc[r] += val and
//   inc[l - 1] -= val (when l > 0); same pop logic.
