# 15 · LFU Cache (pasted batch, Hard) — LC 460
# get(key) = value or -1; put(key, value) inserts or updates. When full, evict the least
# frequently used key, ties -> least recently used. get and put both count as a use.
# Both O(1)
#
# clarify:
# capacity 0 -> put is a no-op
# put on an existing key -> update value, counts as a use
# a new key -> freq 1

from collections import OrderedDict


# core idea: one recency list per freq; a use moves the key to the freq + 1 list;
# min_freq points at the list to evict from
# T: O(1) per op, S: O(capacity)
class LFUCache:
    def __init__(self, capacity):
        # max items kept
        self.cap = capacity
        # smallest freq with a non-empty list
        self.min_freq = 0
        # key -> its value
        self.vals = {}
        # key -> num of uses
        self.freq = {}
        # freq -> OrderedDict of keys with that freq, least recently used first
        self.freq_keys = {}

    # key goes to the end (most recent) of the list for f
    def add_key(self, key, f):
        if f not in self.freq_keys:
            self.freq_keys[f] = OrderedDict()
        self.freq_keys[f][key] = True

    # move key from its freq list to the freq + 1 list
    def touch(self, key):
        f = self.freq[key]
        del self.freq_keys[f][key]
        if f == self.min_freq and len(self.freq_keys[f]) == 0:
            self.min_freq += 1
        self.freq[key] = f + 1
        self.add_key(key, f + 1)

    def get(self, key):
        if key not in self.vals:
            return -1
        self.touch(key)
        return self.vals[key]

    def put(self, key, value):
        if self.cap == 0:
            return
        if key in self.vals:
            self.vals[key] = value
            self.touch(key)
            return
        if len(self.vals) == self.cap:
            # least recent key in the least frequent list
            victim, _ = self.freq_keys[self.min_freq].popitem(last=False)
            del self.vals[victim]
            del self.freq[victim]
        self.vals[key] = value
        self.freq[key] = 1
        self.add_key(key, 1)
        # a new key is always the least frequent
        self.min_freq = 1


def check(label, ok):
    print(label, "PASS" if ok else "FAIL")


def main():
    cache = LFUCache(2)
    cache.put(1, 1)
    cache.put(2, 2)
    cache.put(3, 3)
    check("tie-evicts-lru", cache.get(1) == -1)
    check("hit", cache.get(2) == 2)
    cache.put(4, 4)
    check("evict-least-freq", cache.get(3) == -1)
    cache.put(2, 20)
    check("update", cache.get(2) == 20)


main()

# follow-up:
# 1. why not a heap of (freq, time) -> O(log n) per op plus stale entries
# 2. no OrderedDict -> one doubly linked list per freq, like ref/must/07-lru-cache
# 3. LRU (LC 146) -> the same thing with a single list
