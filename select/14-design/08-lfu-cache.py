# LC 460 LFU Cache (Hard). Pattern: list per frequency + min_freq.
# get(key) returns the value or -1; put(key, value) inserts or updates. When full,
# evict the least frequently used key, ties -> least recently used. get and put both
# count as a use. Both O(1) average.

from collections import OrderedDict
from collections import defaultdict


class Entry:
    def __init__(self, val, freq):
        self.val = val
        self.freq = freq


# One recency list per frequency; a use moves the key to the next frequency's list.
# T: O(1) average per op, S: O(capacity).
class LFUCache:
    def __init__(self, capacity):
        # max items kept
        self.cap = capacity
        # smallest freq with a non-empty list
        self.min_freq = 0
        # key -> its value and freq
        self.entries = {}
        # freq -> keys with that freq as an ordered set (key -> None), least recently
        # used first; OrderedDict stands in for list + iterator: O(1) delete by key
        self.freq_keys = defaultdict(OrderedDict)

    # move key from its freq list to the most recent end of the freq + 1 list
    def touch(self, key):
        entry = self.entries[key]
        keys = self.freq_keys[entry.freq]
        del keys[key]
        if entry.freq == self.min_freq and not keys:
            self.min_freq += 1
        entry.freq += 1
        self.freq_keys[entry.freq][key] = None

    def get(self, key):
        if key not in self.entries:
            return -1
        self.touch(key)
        return self.entries[key].val

    def put(self, key, value):
        if key in self.entries:
            self.entries[key].val = value
            self.touch(key)
            return
        if len(self.entries) == self.cap:
            # least recent key in the least frequent list
            victim = self.freq_keys[self.min_freq].popitem(last=False)[0]
            del self.entries[victim]
        self.freq_keys[1][key] = None
        self.entries[key] = Entry(value, 1)
        # a new key is always the least frequent
        self.min_freq = 1


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    cache = LFUCache(2)
    cache.put(1, 1)
    cache.put(2, 2)
    cache.put(3, 3)
    test("tie-evicts-lru", cache.get(1), -1)
    test("hit", cache.get(2), 2)
    cache.put(4, 4)
    test("evict-least-freq", cache.get(3), -1)
    cache.put(2, 20)
    test("update", cache.get(2), 20)


main()

# follow-up: why not a heap of (freq, time) -> O(log n) per op plus stale entries;
#   per-freq lists + min_freq give O(1)
# follow-up: LRU (LC 146) -> the same thing with a single list
