// impl LRU cache with get(key) (-1 on miss), put(key, value)
// evict the least recently used key when full

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

class LRUCache {
private:
    struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;
    };

    int cap;
    Node head{}; // head.next = most recently
    Node tail{}; // tail.prev = least recently
    unordered_map<int, Node*> nodes; // key -> &Node in list

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
    LRUCache(int cap) : cap(cap) {
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

    LRUCache(const LRUCache&) = delete; // no copy construct
    LRUCache& operator=(const LRUCache&) = delete; // no copy assign

    // T: O(1) avg. hash-lookup + pushFront()
    int get(int key) {
        auto it = nodes.find(key); // find not [], missing doesn't insert
        if (it == nodes.end()) { return -1; }
        remove(it->second);
        pushFront(it->second);
        return it->second->value;
    }

    // T: O(1) avg. hash-lookup + update-inplace + pushFront() + optional evict LRU
    void put(int key, int value) {
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->value = value;
            remove(it->second);
            pushFront(it->second);
            return;
        }

        Node* node = new Node{key, value, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);

        if (static_cast<int>(nodes.size()) > cap) {
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
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1, "evict LRU");
    c.put(1, 10);
    check(c.get(1) == 10 && c.get(3) == 3, "update");
    LRUCache z(0);
    z.put(1, 1);
    check(z.get(1) == -1, "capacity 0");
}
