// LC 384 Shuffle an Array: given an integer array, shuffle() returns a uniformly random
// permutation of it and reset() returns the original configuration.
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::set;
using std::rand;
using std::swap;
using std::sort;

class ArrayShuffler {
    vector<int> original;   // the array as given, returned by reset

public:
    ArrayShuffler(const vector<int>& nums) : original(nums) {}

    // T: O(n), S: O(n) for the copy.
    vector<int> reset() { return original; }

    // Fisher-Yates: slot i takes a uniform pick among the i + 1 values not yet placed.
    // T: O(n), S: O(n).
    vector<int> shuffle() {
        vector<int> a = original;
        for (int i = (int)a.size() - 1; i > 0; i--) {
            // j may equal i: a value is allowed to stay put
            swap(a[i], a[rand() % (i + 1)]);
        }
        return a;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ArrayShuffler s({1, 2, 3});
    vector<int> sorted = s.shuffle();
    sort(sorted.begin(), sorted.end());
    check(sorted == vector<int>{1, 2, 3}, "permutation");
    check(s.reset() == vector<int>{1, 2, 3}, "reset");
    set<vector<int>> orders;   // distinct permutations seen so far
    for (int i = 0; i < 1000; i++) { orders.insert(s.shuffle()); }
    check(orders.size() == 6, "all-orders");
}
