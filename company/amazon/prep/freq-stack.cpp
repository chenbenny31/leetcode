// push(x), pop() removes and return the most freq val
// if tie return most recently pushed one

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
    vector<int> vals;// hold vals in push order
    unordered_map<int, int> freq;

public:
    void push(int val) {
        vals.push_back(val);
        freq[val]++;
    }

    int pop() {
        int maxCnt = 0;

        for (const auto& [val, cnt] : freq) {
            maxCnt = max(maxCnt, cnt);
        }

        int i = vals.size() - 1;
        while (freq[vals[i]] != maxCnt) { i--; }
        int v = vals[i];

        vals.erase(vals.begin() + i);
        freq[v]--;
        return v;
    }
};

// group[f] = vals with f counts, in push order
class FreqStackGroups {
private:
    unordered_map<int, int> freq; // val -> cnt
    unordered_map<int, vector<int>> group; // cnt -> push-order
    int maxFreq = 0;

public:
    void push(int val) {
        freq[val]++;
        int f = freq[val];
        group[f].push_back(val);
        maxFreq = max(maxFreq, f);
    }

    int pop() {
        int v = group[maxFreq].back();
        group[maxFreq].pop_back();
        freq[v]--;
        if (group[maxFreq].empty()) { maxFreq--; }
        return v;
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
