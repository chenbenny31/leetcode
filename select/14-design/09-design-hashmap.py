# LC 706 Design HashMap (Easy). Pattern: separate chaining.
# No built-in hash tables. put(key, value) inserts or updates, get(key) returns -1 if
# absent, remove(key). Keys are non-negative ints.


# Fixed prime number of buckets, each a small list of [key, value].
# T: O(1) average per op, S: O(n + B) for n keys and B buckets.
class MyHashMap:
    def __init__(self):
        # buckets[h] = [key, value] pairs (lists, so the value can be updated in place)
        # whose key hashes to h
        self.buckets = [[] for _ in range(1009)]

    def hash(self, key):
        return key % len(self.buckets)

    def put(self, key, value):
        bucket = self.buckets[self.hash(key)]
        for entry in bucket:
            # existing key: update instead of appending a duplicate
            if entry[0] == key:
                entry[1] = value
                return
        bucket.append([key, value])

    def get(self, key):
        for entry in self.buckets[self.hash(key)]:
            if entry[0] == key:
                return entry[1]
        return -1

    def remove(self, key):
        bucket = self.buckets[self.hash(key)]
        for i in range(len(bucket)):
            if bucket[i][0] == key:
                # order inside a bucket doesn't matter: overwrite with last, pop last
                bucket[i] = bucket[-1]
                bucket.pop()
                return


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    hash_map = MyHashMap()
    hash_map.put(1, 1)
    test("miss", hash_map.get(2), -1)
    hash_map.put(1, 7)
    test("update", hash_map.get(1), 7)
    hash_map.put(1010, 5)
    test("same-bucket", hash_map.get(1010), 5)
    hash_map.remove(1)
    test("remove", hash_map.get(1), -1)
    test("neighbor-kept", hash_map.get(1010), 5)


main()

# follow-up: grow on load -> track count; past 0.75 * B double B and reinsert every
#   pair (its bucket depends on B), O(1) amortized:
#   self.count += 1
#   if self.count > 0.75 * len(self.buckets):
#       self.rehash()
# follow-up: open addressing -> linear probing is cache-friendly but needs tombstones
#   on remove and load < ~0.7; chaining tolerates higher load
