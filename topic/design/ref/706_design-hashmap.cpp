// LC 706 Design HashMap: int keys and values, without a built-in hash table.
// put(key, value) inserts or updates; get(key) returns the value or -1; remove(key)
// deletes the mapping if present.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::pair;
using std::string;
using std::vector;

class MyHashMap {
    // bucket = chain of (key, value) pairs whose key hashes there; a fixed bucket count
    // is enough here (a real map rehashes when the load factor grows)
    vector<vector<pair<int, int>>> table;

public:
    MyHashMap() : table(769) {}

    // separate chaining; key % buckets is the hash.
    // T: O(chain) = O(n / 769) average, S: O(n + 769).
    void put(int key, int value) {
        for (auto& [k, v] : table[key % 769]) {
            if (k == key) {
                v = value;
                return;
            }
        }
        table[key % 769].push_back({key, value});
    }

    int get(int key) {
        for (const auto& [k, v] : table[key % 769]) {
            if (k == key) { return v; }
        }
        return -1;
    }

    void remove(int key) {
        auto& chain = table[key % 769];
        int n = chain.size();
        for (int i = 0; i < n; i++) {
            if (chain[i].first != key) { continue; }
            // chain order does not matter: overwrite with the last entry
            chain[i] = chain.back();
            chain.pop_back();
            return;
        }
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    MyHashMap m;
    check(m.get(1) == -1, "miss");
    m.put(1, 10);
    m.put(1, 11);
    check(m.get(1) == 11, "overwrite");
    m.put(770, 20);
    m.put(1539, 30);
    m.remove(1);
    check(m.get(1) == -1 && m.get(770) == 20 && m.get(1539) == 30, "collide-remove");
    m.remove(1);
    check(m.get(770) == 20, "remove-absent");
}
