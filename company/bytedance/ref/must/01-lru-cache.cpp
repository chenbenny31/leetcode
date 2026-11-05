// ByteDance (hack2hire) / LC 146: LRU cache with get(key) (-1 on miss) and
// put(key, value); when full, put evicts the least recently used key. Both O(1) average.
// Follow-up: per-entry TTL, put(key, value, ttl, now) / get(key, now); ttl 0 = never.

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

// Doubly linked list in recency order + map key -> node. T: O(1) average per op,
// S: O(cap).
class LRUCache {
    struct Node {
        int key;
        int val;
        Node* prev;
        Node* next;
    };
    // max items kept
    int cap;
    // sentinels: head.next = most recently used, tail.prev = least recently used
    Node head{};
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

    // the destructor owns the nodes, so no copies
    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    int get(int key) {
        // find, not []: a miss must not insert
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        remove(it->second);
        pushFront(it->second);
        return it->second->val;
    }

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

// Follow-up: same list + map; each node also stores its expiry time.
// T: O(1) average, O(n) when put has to purge, S: O(cap).
class LRUCacheTTL {
    struct Node {
        int key;
        int val;
        // absolute expiry time; 0 = never expires
        long long expireAt;
        Node* prev;
        Node* next;
    };
    // max items kept
    int cap;
    // sentinels: head.next = most recently used, tail.prev = least recently used
    Node head{};
    Node tail{};
    // key -> its node in the list
    unordered_map<int, Node*> nodes;

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

    bool expired(Node* node, long long now) {
        return node->expireAt != 0 && node->expireAt <= now;
    }

public:
    LRUCacheTTL(int capacity) : cap(capacity) {
        head.next = &tail;
        tail.prev = &head;
    }

    ~LRUCacheTTL() {
        Node* cur = head.next;
        while (cur != &tail) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    LRUCacheTTL(const LRUCacheTTL&) = delete;
    LRUCacheTTL& operator=(const LRUCacheTTL&) = delete;

    // lazy expiry: an expired hit is dropped and reported as a miss
    int get(int key, long long now) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        Node* node = it->second;
        if (expired(node, now)) {
            remove(node);
            nodes.erase(key);
            delete node;
            return -1;
        }
        remove(node);
        pushFront(node);
        return node->val;
    }

    // when full, purge expired entries first; evict the LRU only if still over cap
    void put(int key, int value, int ttl, long long now) {
        long long expireAt = ttl == 0 ? 0 : now + ttl;
        auto it = nodes.find(key);
        if (it != nodes.end()) {
            it->second->val = value;
            it->second->expireAt = expireAt;
            remove(it->second);
            pushFront(it->second);
            return;
        }
        if ((int)nodes.size() >= cap) {
            Node* cur = tail.prev;
            while (cur != &head) {
                // saved before cur may be freed
                Node* prv = cur->prev;
                if (expired(cur, now)) {
                    remove(cur);
                    nodes.erase(cur->key);
                    delete cur;
                }
                cur = prv;
            }
        }
        Node* node = new Node{key, value, expireAt, nullptr, nullptr};
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
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1) == 1, "hit");
    c.put(3, 3);
    check(c.get(2) == -1, "evict-2");
    c.put(4, 4);
    check(c.get(1) == -1, "evict-1");
    check(c.get(3) == 3 && c.get(4) == 4, "keep-3-4");

    LRUCacheTTL t(2);
    t.put(1, 1, 0, 0);
    t.put(2, 2, 5, 0);
    check(t.get(2, 4) == 2, "ttl-alive");
    t.put(3, 3, 0, 10);
    check(t.get(1, 10) == 1 && t.get(2, 10) == -1 && t.get(3, 10) == 3, "ttl-purge");
}

// follow-up: per-key TTL -> see LRUCacheTTL above; expireAt per node, lazy drop on get,
//   purge expired before evicting the LRU
// follow-up: thread-safe -> one mutex around get and put (get reorders the list too);
//   scale with N shards by hash(key) % N, each its own LRU + lock
//   mutex mu;
//   int get(int key) { lock_guard<mutex> lock(mu); /* same body */ }
// follow-up: LFU (LC 460) -> key -> {val, freq, iter}, freq -> list<key>, minFreq;
//   evict back of list[minFreq], O(1)
