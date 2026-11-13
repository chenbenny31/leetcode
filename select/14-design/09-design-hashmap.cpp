// LC 706 Design HashMap (Easy) — separate chaining
// no built-in hash tables; put(key, value) inserts / updates, get(key) = -1 if absent
// remove(key); keys are non-negative ints
//
// clarify:
// key range -> non-negative int, key % B is a valid idx
// expected num of keys -> picks the bucket cnt (1009 here)
// resizing needed? -> no; follow-up
// remove of a missing key -> no-op

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// core idea: fixed prime num of buckets, each a small vector of (key, value), linear scan
// T: O(1) average per op, S: O(n + B) — n keys, B buckets
class MyHashMap {
    // buckets[h] = (key, value) pairs whose key hashes to h
    vector<vector<pair<int, int>>> buckets;

    int hash(int key) { return key % (int)buckets.size(); }

public:
    MyHashMap() : buckets(1009) {}

    void put(int key, int value) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (auto& [storedKey, storedValue] : bucket) {
            // existing key: update, do not append a dup
            if (storedKey == key) {
                storedValue = value;
                return;
            }
        }
        bucket.push_back({key, value});
    }

    int get(int key) {
        for (const auto& [storedKey, storedValue] : buckets[hash(key)]) {
            if (storedKey == key) { return storedValue; }
        }
        return -1;
    }

    void remove(int key) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (auto& entry : bucket) {
            if (entry.first == key) {
                // order inside a bucket doesn't matter: overwrite w/ back, pop back
                entry = bucket.back();
                bucket.pop_back();
                return;
            }
        }
    }
};

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    MyHashMap map;
    map.put(1, 1);
    map.put(1, 7);
    test("update", map.get(1), 7);
    map.put(1010, 5);
    test("same-bucket", map.get(1010), 5);
    map.remove(1);
    test("remove", map.get(1), -1);
    test("neighbor-kept", map.get(1010), 5);
    return 0;
}

// follow-up:
// grow on load -> track key cnt; past 0.75 * B double B and reinsert all; O(1) amortized
// open addressing -> linear probing: cache-friendly, needs tombstones and load < ~0.7
// negative / non-int keys -> hash to unsigned first, then % B
