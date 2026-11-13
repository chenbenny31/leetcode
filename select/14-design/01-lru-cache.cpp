// LC 146 LRU Cache (Medium) — doubly linked list + hash map
// get(key) = value or -1; put(key, value) inserts / updates, evicts LRU key when over cap
// both O(1) average
//
// clarify:
// get counts as a use -> get moves node to front
// put on an existing key -> update + refresh
// capacity 0 -> put is a no-op
// single-threaded -> no lock; thread-safe -> follow-up

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

// core idea: list in recency order + map key -> node; use = move to front, evict at tail
// T: O(1) average per op, S: O(capacity)
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

    // unlink only; caller keeps or frees the node
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
            // node stores its key so the map entry can be erased
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

// follow-up:
// STL version -> list of (key, value) + map key -> list iterator; splice to front in O(1)
// setCapacity(newCap) -> store it, evict from tail while size > cap
// thread-safe -> one mutex around get and put (get reorders the list too)
// scale out -> N shards by hash(key) % N, each its own LRU + lock
