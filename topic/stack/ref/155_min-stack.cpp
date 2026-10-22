// LC 155 Min Stack: design a stack with push, pop, top and getMin, each O(1).
// pop, top and getMin are only called on a non-empty stack.
// Follow-up: store a single number per entry instead of a (value, min) pair.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

// Each entry remembers the min at push time. T: O(1) per op, S: O(n).
class MinStack {
    vector<pair<int, int>> stk;   // (value, min of this entry and everything below)

public:
    void push(int val) {
        int curMin = val;
        if (!stk.empty()) { curMin = min(val, stk.back().second); }
        stk.push_back({val, curMin});
    }

    void pop() { stk.pop_back(); }

    int top() { return stk.back().first; }

    int getMin() { return stk.back().second; }
};

// Difference trick: store val - min; a negative entry marks a push that set a new min
// and lets pop recover the previous min. T: O(1) per op, S: O(n).
class MinStackDiff {
    vector<long long> stk;   // val minus the min before this push; 0 for the first
    long long mn = 0;        // current min

public:
    void push(int val) {
        if (stk.empty()) {
            stk.push_back(0);
            mn = val;
            return;
        }
        long long diff = val - mn;
        stk.push_back(diff);
        // diff < 0: val is the new min; the diff keeps the old min recoverable
        if (diff < 0) { mn = val; }
    }

    void pop() {
        long long diff = stk.back();
        stk.pop_back();
        if (diff < 0) { mn = mn - diff; }
    }

    int top() {
        long long diff = stk.back();
        if (diff < 0) { return mn; }
        return mn + diff;
    }

    int getMin() { return mn; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MinStack s;
    s.push(5);
    s.push(2);
    s.push(7);
    check(s.top() == 7 && s.getMin() == 2, "push");
    s.pop();
    s.pop();
    check(s.top() == 5 && s.getMin() == 5, "min-restored");
    s.push(5);
    s.push(5);
    s.pop();
    check(s.getMin() == 5, "duplicate-min");
}
