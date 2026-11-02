// return every arrival or on request? every arrival
// tie-break for equal freq -> smaller val first
// fewer than k distinct val -> return all
// val range -> signed 32-bit, hash-map over array
// size? n, k <= 1e5
// static version first? yes, then stream

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

// online algo
class TopKStream {
private:
    int k;
    // S: O(d), d=uniq-vals
    unordered_map<int, int> freq; // freq[val] -> cnt
    set<pair<int, int>> ranked; // (-cnt, val), begin with most freq, then smaller val on tie

public:
    TopKStream(int k) : k(k) {}

    void add(int v) { // T: O(log d)
        // remove v's old key first
        if (freq[v] > 0) { ranked.erase({-freq[v], v}); }
        freq[v]++;
        ranked.insert({-freq[v], v});
    }

    vector<int> top() { // T: O(k)
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
