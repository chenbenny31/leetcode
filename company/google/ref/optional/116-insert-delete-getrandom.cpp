// 116 · Insert Delete GetRandom O(1) — LC 380
// Mother: select/14-design/05-insert-delete-getrandom.cpp
// set w/ insert(val) / remove(val), each returns whether the set changed
// getRandom() returns each current element w/ equal probability; all O(1) average
// reported: FancySet w/ O(1) add / delete / getRandom
//
// clarify:
// dups -> none; allowed = follow-up (LC 381)
// getRandom on empty set -> assume never called
// distribution -> uniform over current elements

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

// core idea: dense vector for random access + map val -> position
// remove = move last element into the hole, pop back
// T: O(1) per op, S: O(n) — average, hash map
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

// follow-up:
// dups allowed (LC 381) -> index: val -> set of positions; fix the moved value's set
// better randomness -> mt19937 + uniform_int_distribution instead of rand() % size
