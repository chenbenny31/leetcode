// LC 460 LFU Cache: get(key) returns the value or -1; put(key, value) inserts or
// updates. When full, put first evicts the least frequently used key (ties: the least
// recently used of them). Both get and put count as a use. Both O(1) average.
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::unordered_map;

class LFUCache {
    struct Node {
        int key, val, freq;
        Node* prev;
        Node* next;
    };
    struct Bucket {
        Node head{};   // sentinel; head.next = most recently used
        Node tail{};   // sentinel; tail.prev = least recently used
    };
    int cap;                           // max keys kept
    int minFreq = 0;                   // smallest freq with a non-empty bucket
    unordered_map<int, Node*> nodes;   // key -> its node
    // freq -> recency list of the nodes with that freq (map elements never move)
    unordered_map<int, Bucket> buckets;

    // unlink only; the caller keeps or frees the node
    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // insert at the front of the bucket for node->freq; link its sentinels on first use
    void pushFront(Node* node) {
        Bucket& b = buckets[node->freq];
        if (!b.head.next) {
            b.head.next = &b.tail;
            b.tail.prev = &b.head;
        }
        node->prev = &b.head;
        node->next = b.head.next;
        b.head.next->prev = node;
        b.head.next = node;
    }

    // move a node up one frequency
    void touch(Node* node) {
        remove(node);
        Bucket& b = buckets[node->freq];
        if (node->freq == minFreq && b.head.next == &b.tail) { minFreq++; }
        node->freq++;
        pushFront(node);
    }

public:
    LFUCache(int capacity) : cap(capacity) {}

    ~LFUCache() {
        for (auto& [key, node] : nodes) { delete node; }
    }

    LFUCache(const LFUCache&) = delete;
    LFUCache& operator=(const LFUCache&) = delete;

    // hash lookup + touch. T: O(1) average.
    int get(int key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        touch(it->second);
        return it->second->val;
    }

    // freq buckets of doubly linked lists + minFreq pointer. T: O(1) average,
    // S: O(cap).
    void put(int key, int value) {
        if (cap == 0) { return; }
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            touch(it->second);
            return;
        }
        if ((int)nodes.size() == cap) {
            // least recent node of the least frequent bucket
            Node* victim = buckets[minFreq].tail.prev;
            remove(victim);
            nodes.erase(victim->key);
            delete victim;
        }
        Node* node = new Node{key, value, 1, nullptr, nullptr};
        nodes[key] = node;
        pushFront(node);
        // load-bearing: a new key is always the least frequent
        minFreq = 1;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    LFUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1 && c.get(3) == 3, "evict-least-freq");
    c.put(4, 4);
    check(c.get(1) == -1 && c.get(3) == 3 && c.get(4) == 4, "tie-lru");
    LFUCache z(0);
    z.put(1, 1);
    check(z.get(1) == -1, "cap-zero");
}
