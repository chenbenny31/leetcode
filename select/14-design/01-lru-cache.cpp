// LC 146 LRU Cache (Medium). Pattern: doubly linked list + hash map.
// get(key) returns the value or -1; put(key, value) inserts or updates, and when over
// capacity evicts the least recently used key. Both O(1) average.

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <utility>

using std::cout;
using std::string;
using std::unordered_map;
using std::list;
using std::pair;

// List in recency order + map key -> node; a use moves the node to the front.
// T: O(1) average per op, S: O(capacity).
class LRUCache {
    struct Node {
        int key;
        int val;
        Node* prev;
        Node* next;
    };
    // max items kept
    int cap;
    // sentinel; head.next = most recently used
    Node head{};
    // sentinel; tail.prev = least recently used
    Node tail{};
    // key -> its node in the list
    unordered_map<int, Node*> nodes;

    // unlink only; the caller keeps or frees the node
    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void pushFront(Node* node) {
        node->prev = &head;
        node->next = head.next;
        head.next->prev = node;
        head.next = node;
    }

    // drop least recently used entries while over capacity
    void evictOverflow() {
        while ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            nodes.erase(lru->key);
            delete lru;
        }
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        head.next = &tail;
        tail.prev = &head;
    }

    ~LRUCache() {
        Node* cur = head.next;
        while (cur != &tail) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    int get(int key) {
        // find, not []: a miss must not insert
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        Node* node = it->second;
        remove(node);
        pushFront(node);
        return node->val;
    }

    void put(int key, int value) {
        // capacity 0 stores nothing
        if (cap == 0) { return; }
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            Node* node = it->second;
            node->val = value;
            remove(node);
            pushFront(node);
            return;
        }
        Node* node = new Node{key, value, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        evictOverflow();
    }

    // Follow-up: resize; shrinking evicts least recently used entries until they fit.
    // T: O(1) average per evicted entry.
    void setCapacity(int newCapacity) {
        cap = newCapacity;
        evictOverflow();
    }
};

// Same design on an STL list: splice moves a node to the front in O(1) and keeps
// iterators valid. T: O(1) average per op, S: O(capacity).
class LRUCacheStl {
    // max items kept
    int capacity;
    // {key, value} in recency order; front = most recently used
    list<pair<int, int>> items;
    // key -> its {key, value} node in items
    unordered_map<int, list<pair<int, int>>::iterator> where;

public:
    LRUCacheStl(int capacity) : capacity(capacity) {}

    int get(int key) {
        auto it = where.find(key);
        if (it == where.end()) { return -1; }
        items.splice(items.begin(), items, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = where.find(key);
        if (it != where.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }
        if ((int)items.size() == capacity) {
            // back = least recently used; its key is needed to clean up the map
            where.erase(items.back().first);
            items.pop_back();
        }
        items.push_front({key, value});
        where[key] = items.begin();
    }
};

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    test("hit", cache.get(1), 1);
    cache.put(3, 3);
    test("evict-lru", cache.get(2), -1);
    cache.put(1, 10);
    test("update", cache.get(1), 10);
    cache.put(4, 4);
    test("update-refreshes", cache.get(3), -1);

    LRUCache resized(3);
    resized.put(1, 1);
    resized.put(2, 2);
    resized.put(3, 3);
    test("resize-touch", resized.get(1), 1);
    resized.setCapacity(2);
    test("shrink-evicts-lru", resized.get(2), -1);
    test("shrink-keeps-recent", resized.get(3), 3);
    resized.setCapacity(4);
    resized.put(5, 5);
    resized.put(6, 6);
    test("grow-keeps-all", resized.get(1), 1);

    LRUCache empty(0);
    empty.put(1, 1);
    test("zero-capacity", empty.get(1), -1);

    LRUCacheStl stlCache(2);
    stlCache.put(1, 1);
    stlCache.put(2, 2);
    test("stl-hit", stlCache.get(1), 1);
    stlCache.put(3, 3);
    test("stl-evict-lru", stlCache.get(2), -1);
    stlCache.put(1, 10);
    test("stl-update", stlCache.get(1), 10);
    stlCache.put(4, 4);
    test("stl-update-refreshes", stlCache.get(3), -1);
}

// follow-up: thread-safe -> one mutex around get and put (get reorders the list too);
//   scale with N shards by hash(key) % N, each its own LRU + lock
