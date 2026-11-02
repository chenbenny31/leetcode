// Streaming top k: values arrive one at a time through add(v); top() can be called at
// any moment and returns up to k distinct values seen so far, ordered by frequency
// (high first), smaller value first on ties.
// (fastprep's runner calls add then top after every value.)

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::pair;
using std::unordered_map;

// Online: state is each value's count plus an ordered set keyed by (-count, value).
// add moves one value to its new key; top reads the first k entries.
// T: add O(log d), top O(k), for d distinct values. S: O(d).
class TopKStream {
    int k;
    // freq[v] = how many times v has arrived
    unordered_map<int, int> freq;
    // (-count, value): begin() is the most frequent, smaller value first on ties
    set<pair<int, int>> ranked;

public:
    TopKStream(int k) : k(k) {}

    void add(int v) {
        // remove v's old key before its count changes
        if (freq[v] > 0) { ranked.erase({-freq[v], v}); }
        freq[v]++;
        ranked.insert({-freq[v], v});
    }

    vector<int> top() {
        vector<int> result;
        for (const auto& entry : ranked) {
            if ((int)result.size() == k) { break; }
            result.push_back(entry.second);
        }
        return result;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TopKStream s(2);
    check(s.top().empty(), "empty");
    s.add(2);
    s.add(1);
    check(s.top() == vector<int>{1, 2}, "tie-smaller-first");
    s.add(2);
    check(s.top() == vector<int>{2, 1}, "overtake");
    TopKStream one(1);
    one.add(5);
    one.add(3);
    one.add(3);
    check(one.top() == vector<int>{3}, "k-one");
}
