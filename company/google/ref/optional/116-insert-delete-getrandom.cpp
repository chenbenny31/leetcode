// 116 · Insert Delete GetRandom O(1) — LC 380
// Mother problem: select/14-design/05-insert-delete-getrandom.cpp
//
// Google reports: FancySet with O(1) add / delete / getRandom
//
// Clarify
// - duplicates? → no (set); insert / remove return whether the set changed
// - getRandom on empty? → never called
// - uniform? → each current element equally likely
// - complexity? → O(1) average per op
//
// Core idea
// - vals: dense vector of elements; index: val → position in vals.
// - remove: move vals.back() into the hole, update its index before erasing val (val may
//   be the last element), pop_back.
// - getRandom: vals[rand() % size].
// - T O(1) average per op, S O(n).
//
// Tests: dup insert, remove missing, remove first, remove last
//
// Follow-ups
// - duplicates allowed (LC 381) → index: val → unordered_set of positions; move
//   vals.back() into any position, fix the moved value's set
// - better randomness → mt19937 + uniform_int_distribution
//
//////////////////////////////////////////////////////////////////////////////////////////

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
    set.remove(1);
    test("remove-first", set.getRandom(), 2);
    test("remove-last", set.remove(2), true);
}
