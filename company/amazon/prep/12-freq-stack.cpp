// tie on freq by most recently pushed wins
// never pop call on empty stack
// val range type in int
// no need of peek and size

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

class FreqStack {
private:
    vector<int> stk; // all vals in push order
    unordered_map<int, int> freq; // freq[val] = cnt

public:
    void push(int val) { // T: O(1)
        stk.push_back(val);
        freq[val]++;
    }

    int pop() { // T: O(n)
        int maxCnt = 0;
        for (const auto& [val, cnt] : freq) {
            maxCnt = max(maxCnt, cnt);
        }

        int pos = stk.size() - 1; // pos be last val with maxCnt
        while (freq[stk[pos]] != maxCnt) { pos--; }
        int val = stk[pos];
        stk.erase(stk.begin() + pos);
        freq[val]--;
        return val;
    }
};

// bucket freq, group[cnt] = vals that with cnt in pushed order
class FreqStackGroups {
private:
    unordered_map<int, int> freq;
    unordered_map<int, vector<int>> group;
    int maxCnt = 0;

public:
    void push(int val) { // T: O(1)
        freq[val]++;
        int cnt = freq[val];
        group[cnt].push_back(val);
        maxCnt = max(maxCnt, cnt);
    }

    int pop() { // T: O(1)
        int val = group[maxCnt].back();
        group[maxCnt].pop_back();
        freq[val]--;
        if (group[maxCnt].empty()) { maxCnt--; }
        return val;
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
