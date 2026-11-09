# LC 146 LRU Cache (Medium). Pattern: doubly linked list + hash map.
# get(key) returns the value or -1; put(key, value) inserts or updates, and when over
# capacity evicts the least recently used key. Both O(1) average.


class Node:
    def __init__(self, key, val):
        self.key = key
        self.val = val
        self.prev = None
        self.next = None


# List in recency order + map key -> node; a use moves the node to the front.
# T: O(1) average per op, S: O(capacity).
class LRUCache:
    def __init__(self, capacity):
        # max items kept
        self.cap = capacity
        # sentinel; head.next = most recently used
        self.head = Node(0, 0)
        # sentinel; tail.prev = least recently used
        self.tail = Node(0, 0)
        # key -> its node in the list
        self.nodes = {}
        self.head.next = self.tail
        self.tail.prev = self.head

    # unlink only; the caller keeps or drops the node
    def remove(self, node):
        node.prev.next = node.next
        node.next.prev = node.prev

    def push_front(self, node):
        node.prev = self.head
        node.next = self.head.next
        self.head.next.prev = node
        self.head.next = node

    def get(self, key):
        # a miss must not insert
        if key not in self.nodes:
            return -1
        node = self.nodes[key]
        self.remove(node)
        self.push_front(node)
        return node.val

    def put(self, key, value):
        if key in self.nodes:
            node = self.nodes[key]
            node.val = value
            self.remove(node)
            self.push_front(node)
            return
        node = Node(key, value)
        self.nodes[key] = node
        self.push_front(node)
        if len(self.nodes) > self.cap:
            lru = self.tail.prev
            self.remove(lru)
            del self.nodes[lru.key]


def test(label, got, want):
    print(label, "PASS" if got == want else "FAIL")


def main():
    cache = LRUCache(2)
    cache.put(1, 1)
    cache.put(2, 2)
    test("hit", cache.get(1), 1)
    cache.put(3, 3)
    test("evict-lru", cache.get(2), -1)
    cache.put(1, 10)
    test("update", cache.get(1), 10)
    cache.put(4, 4)
    test("update-refreshes", cache.get(3), -1)


main()

# follow-up: with the stdlib -> collections.OrderedDict: move_to_end(key) on use,
#   popitem(last=False) evicts the LRU
# follow-up: thread-safe -> one threading.Lock around get and put (get reorders the list
#   too); scale with N shards by hash(key) % N, each its own LRU + lock
