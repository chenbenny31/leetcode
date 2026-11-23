class Node:
    def __init__(self, key, val):
        self.key = key
        self.val = val
        self.prev = None
        self.next = None

# list in recent order + dict key->node: move to front is make recent, evict at tail
class LRUCache:
    def __init__(self, cap):
        self.cap = cap
        self.nodes = {}
        self.head = Node(0, 0)
        self.tail = Node(0, 0)
        self.head.next = self.tail
        self.tail.prev = self.head

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
    
    def put(self, key, val):
        if self.cap == 0:
            return
        if key in self.nodes:
            node = self.nodes[key]
            node.val = val
            self.remove(node)
            self.push_front(node)
            return

        node = Node(key, val)
        self.nodes[key] = node
        self.push_front(node)

        if len(self.nodes) > self.cap:
            lru = self.tail.prev
            self.remove(lru)
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


