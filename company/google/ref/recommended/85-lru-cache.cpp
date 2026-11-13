// 85 · LRU Cache — LC 146
// Mother: select/14-design/01-lru-cache.cpp
// get(key) returns the value or -1; put(key, value) inserts or updates
// over capacity -> evict the least recently used key; both O(1) average
// reported: LC 146 ×3, resize at runtime, LRU ×3 (1p3a)
//
// clarify:
// get miss -> must not insert
// put on an existing key -> update + refresh
// capacity 0 -> put is a no-op
// resize at runtime -> follow-up

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

// core idea: doubly linked list in recency order (sentinel head / tail) + map key -> node
// a use unlinks the node and pushes it to the front; overflow evicts tail.prev
// T: O(1) per op, S: O(capacity)
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
        if ((int)nodes.size() > cap) {
            Node* lru = tail.prev;
            remove(lru);
            nodes.erase(lru->key);
            delete lru;
        }
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

    LRUCache empty(0);
    empty.put(1, 1);
    test("zero-capacity", empty.get(1), -1);
}

// follow-up:
// resize at runtime, setCapacity (Google) -> set cap, evict tail.prev while size > cap
// STL version -> list of (key, value) + map key -> list iterator; splice to the front
// thread-safe -> one mutex around get and put (get reorders too)
// less lock contention -> N shards by hash(key) % N, each its own LRU + lock
