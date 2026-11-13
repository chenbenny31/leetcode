// 117 · LFU Cache — LC 460
// Mother: select/optional/14-design/08-lfu-cache.cpp
// get(key) / put(key, value) w/ capacity; full put evicts least frequently used key
// ties -> least recently used; both get and put count as a use
// reported: 1p3a
//
// clarify:
// tie rule -> LRU among the min freq keys
// put on existing key -> update value, counts as a use
// capacity 0 -> put is a no-op
// cost per op -> O(1) average expected

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

using std::cout;
using std::string;
using std::list;
using std::unordered_map;

// core idea: one recency list of keys per freq + minFreq
// use = move key to front of freq + 1 list; evict = back of minFreq list
// T: O(1) per op, S: O(capacity) — average, hash maps
class LFUCache {
    struct Entry {
        int val;
        int freq;
        // position of the key inside freqKeys[freq]
        list<int>::iterator pos;
    };
    int cap;
    // smallest freq with a non-empty list
    int minFreq = 0;
    // key -> its value, freq and list position
    unordered_map<int, Entry> entries;
    // freq -> keys with that freq, most recently used at the front
    unordered_map<int, list<int>> freqKeys;

    // move key from its freq list to the front of the freq + 1 list
    void touch(int key) {
        Entry& entry = entries[key];
        freqKeys[entry.freq].erase(entry.pos);
        if (entry.freq == minFreq && freqKeys[entry.freq].empty()) {
            minFreq++;
        }
        entry.freq++;
        freqKeys[entry.freq].push_front(key);
        entry.pos = freqKeys[entry.freq].begin();
    }

public:
    LFUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if (entries.count(key) == 0) { return -1; }
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

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    LFUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cache.put(3, 3);
    test("tie-evicts-lru", cache.get(1), -1);
    test("hit", cache.get(2), 2);
    cache.put(4, 4);
    test("evict-least-freq", cache.get(3), -1);
    cache.put(2, 20);
    test("update", cache.get(2), 20);
}

// follow-up:
// why not a heap of (freq, time) -> O(log n) per op + stale entries to skip
// LRU (LC 146) -> same w/ a single list and no freq
