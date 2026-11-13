// 112 · LFU Cache — LC 460
// Mother problem: select/optional/14-design/08-lfu-cache.cpp
//
// Google reports: 1p3a
//
// Clarify
// - eviction? → least frequent; ties → least recently used
// - what counts as a use? → both get and put (put on an existing key updates and uses
//   it)
// - capacity 0? → LC allows it; guard put if needed (the code assumes ≥ 1)
// - complexity? → O(1) average per op
//
// Core idea
// - entries: key → (val, freq, iterator into its list); freqKeys: freq → list of keys,
//   most recent at the front; minFreq = smallest non-empty freq.
// - `touch`: move the key from list freq to the front of list freq + 1; if it emptied
//   list minFreq, minFreq++.
// - Evict freqKeys[minFreq].back(); a new key sets minFreq = 1.
// - T O(1) average per op, S O(capacity).
//
// Tests: tie evicts lru, hit, evict least freq, update
//
// Follow-ups
// - why not a heap of (freq, time) → O(log n) per op plus stale entries
// - LRU (LC 146) → the same with a single list
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

using std::cout;
using std::string;
using std::list;
using std::unordered_map;

// One recency list per frequency; a use moves the key to the next frequency's list.
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
