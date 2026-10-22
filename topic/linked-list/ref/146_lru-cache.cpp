// LC 146 LRU Cache: get(key) returns the value or -1; put(key, value) inserts or updates
// and, when over capacity, evicts the least recently used key. Both in O(1) average.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

// Doubly linked list ordered by recency + hash map from key to node; every use moves a
// node to the front, eviction takes the node before the tail sentinel.
class LRUCache {
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
    };
    int cap;                          // max items kept
    Node head{};                      // sentinel; head.next = most recently used
    Node tail{};                      // sentinel; tail.prev = least recently used
    unordered_map<int, Node*> nodes;  // key -> its node in the list

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

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    // T: O(1) average, S: O(1).
    int get(int key) {
        auto it = nodes.find(key);   // find, not [], so a miss inserts nothing
        if (it == nodes.end()) { return -1; }
        remove(it->second);
        pushFront(it->second);
        return it->second->val;
    }

    // T: O(1) average, S: O(1).
    void put(int key, int value) {
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            remove(it->second);
            pushFront(it->second);
            return;
        }
        Node* node = new Node{key, value, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        if ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            nodes.erase(lru->key);
            delete lru;
        }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LRUCache c(2);
    check(c.get(1) == -1, "miss");
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1, "evict");
    c.put(1, 10);
    check(c.get(1) == 10 && c.get(3) == 3, "update");
}
