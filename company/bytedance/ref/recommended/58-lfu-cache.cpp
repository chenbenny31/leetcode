// ByteDance (hack2hire) / LC 460: LFU cache with get (-1 on miss) and put. When full,
// evict the least frequently used key; ties -> the least recently used among them.
// get and put both count as a use.

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

using std::cout;
using std::string;
using std::list;
using std::unordered_map;

// One recency list per frequency + minFreq; a use moves the key to the next list.
// T: O(1) average per op, S: O(capacity).
class LFUCache {
    struct Entry {
        int val;
        int freq;
        // position of the key inside freqKeys[freq]
        list<int>::iterator pos;
    };
    // max items kept
    int cap;
    // smallest freq with a non-empty list
    int minFreq = 0;
    // key -> its value, freq and list position
    unordered_map<int, Entry> entries;
    // freq -> keys with that freq, most recently used at the front
    unordered_map<int, list<int>> freqKeys;

    // move key from its freq list to the front of the freq + 1 list
    void touch(int key) {
        Entry& e = entries[key];
        freqKeys[e.freq].erase(e.pos);
        if (e.freq == minFreq && freqKeys[e.freq].empty()) {
            minFreq++;
        }
        e.freq++;
        freqKeys[e.freq].push_front(key);
        e.pos = freqKeys[e.freq].begin();
    }

public:
    LFUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if (entries.count(key) == 0) {
            return -1;
        }
        touch(key);
        return entries[key].val;
    }

    void put(int key, int value) {
        if (cap == 0) {
            return;
        }
        if (entries.count(key) > 0) {
            entries[key].val = value;
            touch(key);
            return;
        }
        if ((int)entries.size() == cap) {
            // least recent key in the least frequent list
            int victim = freqKeys[minFreq].back();
            freqKeys[minFreq].pop_back();
            entries.erase(victim);
        }
        freqKeys[1].push_front(key);
        entries[key] = {value, 1, freqKeys[1].begin()};
        // a new key is always the least frequent
        minFreq = 1;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LFUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1 && c.get(3) == 3, "evict-least-freq");
    c.put(4, 4);
    check(c.get(1) == -1 && c.get(3) == 3 && c.get(4) == 4, "tie-lru");
    c.put(3, 30);
    check(c.get(3) == 30, "update");
}

// follow-up: LRU (LC 146) -> list<pair<key, val>> + unordered_map<key, list::iterator>,
//   splice to front on use, evict back, O(1)
// follow-up: why not a heap of (freq, time) -> O(log n) per op plus stale entries;
//   per-freq lists + minFreq give O(1)
// follow-up: thread-safe -> one mutex around get/put (get mutates freq, so no shared
//   read lock); sharding by key cuts contention but makes LFU per shard
