// LC 146 LRU Cache (Medium) — doubly linked list + hash map
// get(key) returns the value or -1; put(key, value) inserts or updates, and when over
// capacity evicts the least recently used key. Both O(1) average.
// Clarify: get counts as a use; put on an existing key refreshes it; capacity 0;
//   single-threaded.

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

// Core idea: a list in recency order plus a map key -> node; every use moves the node
// to the front, and eviction takes the node before the tail.
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

    void evictOverflow() {
        while ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            // the node stores its key so the map entry can be erased
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
    return 0;
}

// follow-up (ideas only):
// - STL version -> list of (key, value) in recency order + map key -> list iterator;
//   splice moves a node to the front in O(1) and keeps iterators valid
// - setCapacity(newCapacity) -> store it, then evict from the tail while size > cap;
//   O(1) average per evicted entry, growing evicts nothing
// - thread-safe -> one mutex around get and put (get reorders the list too)
// - scale out -> N shards by hash(key) % N, each its own LRU + lock
