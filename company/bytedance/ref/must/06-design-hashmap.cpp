// ByteDance (hack2hire) / LC 706: design a HashMap without built-in hash tables:
// put(key, value) inserts or updates, get(key) returns -1 if absent, remove(key).
// Keys are non-negative ints. Follow-up: grow and rehash past load factor 0.75.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// Separate chaining over a fixed prime bucket count.
// T: O(1) average per op, S: O(n + B) for n keys and B buckets.
class MyHashMap {
    // buckets[h] = (key, value) pairs whose key hashes to h
    vector<vector<pair<int, int>>> buckets;

    int hash(int key) { return key % (int)buckets.size(); }

public:
    MyHashMap() : buckets(1009) {}

    void put(int key, int value) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (pair<int, int>& kv : bucket) {
            // existing key: update instead of appending a duplicate
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        bucket.push_back({key, value});
    }

    int get(int key) {
        for (const pair<int, int>& kv : buckets[hash(key)]) {
            if (kv.first == key) { return kv.second; }
        }
        return -1;
    }

    void remove(int key) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (pair<int, int>& kv : bucket) {
            if (kv.first == key) {
                // order inside a bucket doesn't matter: overwrite with back, pop back
                kv = bucket.back();
                bucket.pop_back();
                return;
            }
        }
    }
};

// Follow-up: track the count; double the buckets and reinsert past load 0.75.
// T: O(1) amortized per op.
class ResizableHashMap {
    // buckets[h] = (key, value) pairs whose key hashes to h
    vector<vector<pair<int, int>>> buckets;
    // number of stored keys
    int count = 0;

    int hash(int key) { return key % (int)buckets.size(); }

    // every key moves: its bucket index depends on the bucket count. T: O(n + B).
    void rehash() {
        vector<vector<pair<int, int>>> old;
        old.swap(buckets);
        buckets.resize(2 * old.size());
        for (const vector<pair<int, int>>& bucket : old) {
            for (const pair<int, int>& kv : bucket) {
                buckets[hash(kv.first)].push_back(kv);
            }
        }
    }

public:
    ResizableHashMap() : buckets(16) {}

    void put(int key, int value) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (pair<int, int>& kv : bucket) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        bucket.push_back({key, value});
        count++;
        if (count > 0.75 * buckets.size()) { rehash(); }
    }

    int get(int key) {
        for (const pair<int, int>& kv : buckets[hash(key)]) {
            if (kv.first == key) { return kv.second; }
        }
        return -1;
    }

    void remove(int key) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (pair<int, int>& kv : bucket) {
            if (kv.first == key) {
                kv = bucket.back();
                bucket.pop_back();
                count--;
                return;
            }
        }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MyHashMap m;
    m.put(1, 1);
    m.put(2, 2);
    check(m.get(1) == 1 && m.get(3) == -1, "hit and miss");
    m.put(2, 1);
    check(m.get(2) == 1, "update");
    m.remove(2);
    check(m.get(2) == -1, "remove");
    m.put(1010, 5);
    m.remove(1);
    check(m.get(1) == -1 && m.get(1010) == 5, "same bucket");

    ResizableHashMap r;
    for (int key = 0; key < 100; key++) { r.put(key, key * 10); }
    r.remove(50);
    check(r.get(99) == 990 && r.get(50) == -1 && r.get(0) == 0, "survives rehash");
}
