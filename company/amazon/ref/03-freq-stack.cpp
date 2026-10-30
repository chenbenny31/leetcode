// LC 895 FreqStack: push(x); pop() removes and returns the most frequent value, and
// among equally frequent values the one pushed most recently. pop is called only when
// the stack is non-empty.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;

// straightforward: keep every push; pop scans from the top for the first value with the
// max frequency. T: push O(1), pop O(n). S: O(n).
class FreqStack {
    // all values currently held, in push order
    vector<int> stk;
    // freq[x] = copies of x currently held
    unordered_map<int, int> freq;

public:
    void push(int x) {
        stk.push_back(x);
        freq[x]++;
    }

    int pop() {
        int top = 0;
        for (const auto& [x, f] : freq) { top = max(top, f); }
        int i = stk.size() - 1;
        while (freq[stk[i]] != top) { i--; }
        int x = stk[i];
        stk.erase(stk.begin() + i);
        freq[x]--;
        return x;
    }
};

// improved: group[f] = values that reached frequency f, in push order. The k-th copy of x
// sits in group[k], so popping from the highest group gives the right value.
// T: O(1) per op, S: O(n).
class FreqStackGroups {
    // freq[x] = copies of x currently held
    unordered_map<int, int> freq;
    // group[f] = stack of values whose f-th copy is held, newest on top
    unordered_map<int, vector<int>> group;
    int maxFreq = 0;

public:
    void push(int x) {
        freq[x]++;
        int f = freq[x];
        group[f].push_back(x);
        maxFreq = max(maxFreq, f);
    }

    int pop() {
        int x = group[maxFreq].back();
        group[maxFreq].pop_back();
        freq[x]--;
        if (group[maxFreq].empty()) { maxFreq--; }
        return x;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FreqStack a;
    FreqStackGroups b;
    for (int x : {1, 2, 1}) {
        a.push(x);
        b.push(x);
    }
    check(a.pop() == 1 && b.pop() == 1, "most-freq");
    check(a.pop() == 2 && b.pop() == 2, "tie-recent");
    check(a.pop() == 1 && b.pop() == 1, "last");
    for (int x : {3, 3, 4, 4}) {
        a.push(x);
        b.push(x);
    }
    check(a.pop() == 4 && b.pop() == 4, "tie-again");
    check(a.pop() == 3 && b.pop() == 3, "freq-wins");
}
