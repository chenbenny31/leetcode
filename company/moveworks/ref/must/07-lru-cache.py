# 07 · LRU Cache (ServiceNow intern 2026 onsite, Medium) — LC 146
# get(key) = value or -1; put(key, value) inserts / updates, evicts the least recently
# used key when over capacity; both O(1)
#
# clarify:
# get counts as a use -> get moves the node to the front
# put on an existing key -> update + refresh
# capacity 0 -> put is a no-op
# OrderedDict allowed? -> ask; this hand-written list is what they usually want


class Node:
    def __init__(self, key, val):
        self.key = key
        self.val = val
        self.prev = None
        self.next = None


# core idea: list in recency order + dict key -> node; use = move to front, evict at tail
# T: O(1) per op, S: O(capacity)
class LRUCache:
    def __init__(self, capacity):
        # max items kept
        self.cap = capacity
        # key -> its node in the list
        self.nodes = {}
        # sentinels: head.next = most recently used, tail.prev = least recently used
        self.head = Node(0, 0)
        self.tail = Node(0, 0)
        self.head.next = self.tail
        self.tail.prev = self.head

    # unlink only
    def remove(self, node):
        node.prev.next = node.next
        node.next.prev = node.prev

    def push_front(self, node):
        node.prev = self.head
        node.next = self.head.next
        self.head.next.prev = node
        self.head.next = node

    def get(self, key):
        if key not in self.nodes:
            return -1
        node = self.nodes[key]
        self.remove(node)
        self.push_front(node)
        return node.val

    def put(self, key, value):
        if self.cap == 0:
            return
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
            # the node stores its key so the dict entry can be deleted
            del self.nodes[lru.key]


def check(label, ok):
    print(label, "PASS" if ok else "FAIL")


def main():
    cache = LRUCache(2)
    cache.put(1, 1)
    cache.put(2, 2)
    check("hit", cache.get(1) == 1)
    cache.put(3, 3)
    check("evict-lru", cache.get(2) == -1)
    cache.put(1, 10)
    check("update", cache.get(1) == 10)
    cache.put(4, 4)
    check("update-refreshes", cache.get(3) == -1)


main()

# follow-up:
# 1. OrderedDict -> move_to_end(key) on a use, popitem(last=False) to evict
# 2. set_capacity(new_cap) -> store it, evict from the tail while size > cap
# 3. thread-safe -> one lock around get and put (get reorders the list too)
# 4. scale out -> N shards by hash(key) % N, each its own LRU + lock
# 5. least frequently used instead -> ref/recommended/15-lfu-cache
