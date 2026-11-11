// LC 895 Maximum Frequency Stack (Hard). Pattern: frequency -> stack of values.
// push(val); pop() removes and returns the most frequent value; among equally frequent
// values, the one pushed most recently.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// One stack per frequency level: a value with count c sits in stacks 1..c, so the top of
// the highest stack is always the answer. T: O(1) per op, S: O(pushes).
class FreqStack {
    // freq[val] = current count of val
    unordered_map<int, int> freq;
    // group[count] = values that reached that count, in push order (top = most recent)
    unordered_map<int, vector<int>> group;
    // highest count present
    int maxFreq = 0;

public:
    void push(int val) {
        freq[val]++;
        int count = freq[val];
        if (count > maxFreq) { maxFreq = count; }
        group[count].push_back(val);
    }

    int pop() {
        int val = group[maxFreq].back();
        group[maxFreq].pop_back();
        freq[val]--;
        // the level is empty: the next highest count is exactly one less
        if (group[maxFreq].empty()) { maxFreq--; }
        return val;
    }
};

void test(const string& label, const vector<int>& pushes, const vector<int>& want) {
    FreqStack stack;
    for (int val : pushes) { stack.push(val); }
    vector<int> got;
    for (int i = 0; i < (int)want.size(); i++) { got.push_back(stack.pop()); }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("most-frequent", {1, 2, 1}, {1, 2, 1});
    test("tie-most-recent", {1, 2, 2, 1}, {1, 2, 2, 1});
    test("plain-stack", {3, 4, 5}, {5, 4, 3});
}

// follow-up: why not a heap of (freq, pushTime, val) -> works, O(log n) per op, but stale
//   entries pile up; the per-level stacks give O(1)
// follow-up: peek() -> group[maxFreq].back() without popping
