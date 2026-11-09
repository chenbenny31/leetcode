// LC 380 Insert Delete GetRandom O(1) (Medium). Pattern: vector + index map, swap-pop.
// insert(val) / remove(val) return whether the set changed; getRandom() returns a
// uniformly random current element (called only when non-empty). All O(1) average.

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

// Dense vector for random access; remove moves the last element into the hole.
// T: O(1) average per op, S: O(n).
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
    int pick = set.getRandom();
    test("random-member", pick == 1 || pick == 2, true);
    test("remove-first", set.remove(1), true);
    test("one-left", set.getRandom(), 2);
    test("remove-last", set.remove(2), true);
}

// follow-up: duplicates allowed (LC 381) -> index: val -> unordered_set of positions;
//   remove takes any position, moves vals.back() there, fixes the moved value's set
// follow-up: better randomness -> mt19937 rng + uniform_int_distribution
