// kv types and range? keys are non-neg, both in
// neg keys allow? no
// missing return? -1
// put on existing -> update in place
// freq of ops -> fixed bucket of 1e4 ops, resize (follow-up)
// thread safe? no, if so lock for map OR one lock per bucket

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// sep chaining over a fixed prime bucket, T: O(1) avg, S: O(n+B), B=buckets
class HashMap {
private:
    vector<vector<pair<int, int>>> buckets; // buckets[h] = (k, v) k hashest to h
    int hash(int key) {
        return key % (int)buckets.size();
    }

public:
    HashMap() : buckets(1009) {}

    void put(int key, int value) {
        vector<pair<int, int>>& bucket = buckets[hash(key)];
        for (pair<int, int>& kv : bucket) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        bucket.push_back({key ,value});
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
                kv = bucket.back(); // overwrite with back
                bucket.pop_back();
                return;
            }
        }
    }
};

// follow-up:
class ResizeableHashMap {
private:
    vector<vector<pair<int, int>>> buckets;
    int count = 0;

    int hash(int key) {
        return key % (int)buckets.size();
    }

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
    ResizeableHashMap() : buckets(16) {}

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
    HashMap m;
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

    ResizeableHashMap r;
    for (int key = 0; key < 100; key++) { r.put(key, key * 10); }
    r.remove(50);
    check(r.get(99) == 990 && r.get(50) == -1 && r.get(0) == 0, "survives rehash");
}
