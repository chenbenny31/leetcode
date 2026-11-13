// LC 380 Insert Delete GetRandom O(1) (Medium) — vector + index map, swap-pop
// insert(val) / remove(val) return whether the set changed; getRandom() returns a
// uniformly random current element. All O(1) average.
// Clarify: no duplicates (it is a set); getRandom only when non-empty; uniform over
//   the current elements.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::rand;

// Core idea: dense vector for random access plus a map val -> position; remove moves
// the last element into the hole. T: O(1) average per op, S: O(n).
class RandomizedSet {
    // the elements, in arbitrary order
    vector<int> vals;
    // val -> its position in vals
    unordered_map<int, int> index;

public:
    bool insert(int val) {
        if (index.count(val)) { return false; }
        index[val] = vals.size();
        vals.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = index.find(val);
        if (it == index.end()) { return false; }
        int pos = it->second;
        int last = vals.back();
        vals[pos] = last;
        // before erase: val may be the last element itself
        index[last] = pos;
        vals.pop_back();
        index.erase(val);
        return true;
    }

    int getRandom() { return vals[rand() % vals.size()]; }
};

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    RandomizedSet set;
    set.insert(1);
    set.insert(2);
    test("dup-insert", set.insert(1), false);
    test("remove-missing", set.remove(3), false);
    test("remove-first", set.remove(1), true);
    test("one-left", set.getRandom(), 2);
    return 0;
}

// follow-up (ideas only):
// - duplicates allowed (LC 381) -> map val -> set of positions; remove takes any
//   position, moves vals.back() there and fixes the moved value's position set
// - better randomness -> mt19937 with uniform_int_distribution (rand() % n is biased)
