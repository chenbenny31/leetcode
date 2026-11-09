# LC 981 Time Based Key-Value Store (Medium). Pattern: per-key history + binary search.
# set(key, value, timestamp) with strictly increasing timestamps; get(key, timestamp)
# returns the value set at the largest timestamp <= timestamp, or "" if none.

from collections import defaultdict


# Appending keeps each key's history sorted; get binary searches it.
# T: O(1) set, O(log k) get for k versions of the key; S: O(total sets).
class TimeMap:
    def __init__(self):
        # store[key] = (timestamp, value) pairs in increasing timestamp order
        self.store = defaultdict(list)

    def set(self, key, value, timestamp):
        self.store[key].append((timestamp, value))

    def get(self, key, timestamp):
        # in, not []: a miss must not insert into the defaultdict
        if key not in self.store:
            return ""
        entries = self.store[key]
        # last index with entry time <= timestamp, -1 if none
        found = -1
        lo = 0
        hi = len(entries) - 1
        while lo <= hi:
            mid = (lo + hi) // 2
            if entries[mid][0] <= timestamp:
                found = mid
                lo = mid + 1
            else:
                hi = mid - 1
        if found == -1:
            return ""
        return entries[found][1]


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    store = TimeMap()
    store.set("price", "10", 5)
    store.set("price", "12", 9)
    test("before-first", store.get("price", 4), "")
    test("exact", store.get("price", 5), "10")
    test("between", store.get("price", 8), "10")
    test("after-last", store.get("price", 100), "12")
    test("missing-key", store.get("stock", 7), "")


main()

# follow-up: timestamps out of order -> per key, parallel sorted lists times / values
#   with bisect.insort-style inserts (O(k) set, no ordered map in Python); get:
#   i = bisect_right(times, t), return "" if i == 0 else values[i - 1]
# follow-up: thread-safe -> threading.Lock around set and get (no stdlib
#   readers-writer lock)
