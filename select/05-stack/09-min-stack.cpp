// LC 155 Min Stack (Medium) — stack of (value, min so far) pairs
// stack w/ push, pop, top, getMin (smallest stored), each O(1)
//
// clarify:
// pop / top / getMin on empty -> assume never called
// dup of the min -> each entry keeps its own min, pop stays correct
// value range -> matters only for the O(1) extra space trick (follow-up)

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

// core idea: store w/ each value the min of everything at or below it; top pair = cur min
// T: O(1) per op, S: O(n)
class MinStack {
    // (value, min from bottom up to this entry)
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

// follow-up:
// max stack w/ popMax (LC 716) -> list + ordered map value -> list iterators, O(log n)
// O(1) extra space -> store 2 * x - min when x is new min; prev min = 2 * min - stored
// min queue -> monotonic deque: pop larger from back on push, pop front when it leaves
// increment bottom k (LC 1381) -> lazy add[k - 1] += val; on pop pass to add[top - 1]
// queue w/ two stacks (LC 232) -> push to in; out empty -> pour in to out; amortized O(1)
// frequency stack (LC 895) -> map value -> cnt + one stack per cnt; pop from max cnt
