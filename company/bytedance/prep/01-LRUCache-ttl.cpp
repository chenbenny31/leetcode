// cap range, be 0? -> cap>=1
// key/val range -> non-neg, -1 can mean "miss"
// is get count a use? yes
// put on existing key -> update and make most recent
// thread-safety: single-thread, concurrent -> one mutex around each public call
// ttl = 0 means never expires
// entry expire? now >= expireAt
// full, evict which -> expired go first then LRU

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

class LRUCache {
private:
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
    };
    int cap;
    Node head{};
    Node tail{};
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

    int get(int key) {
        auto it = nodes.find(key);
        if (it == nodes.end()) { return -1; }
        remove(it->second);
        pushFront(it->second);
        return it->second->val;
    }

    void put(int key, int value) {
        auto it = nodes.find(key);
        if (it != nodes.end()) { // update existing
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

class LRUCacheTTL {
private:
    struct Node {
        int key, val;
        long long expireAt; // =now+ttl, 0 means never
        Node* prev;
        Node* next;
    };
    int cap;
    Node head{};
    Node tail{};
    unordered_map<int, Node*> nodes; // key -> node ptr

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
    LRUCacheTTL(int cap) : cap(cap) {
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

    LRUCacheTTL(const LRUCacheTTL&) = delete; // no copy construct
    LRUCacheTTL& operator=(const LRUCacheTTL&) = delete; // no copy assign

    // lazy expiry, T: O(1) avg
    int get(int key, long long now) {
        auto it = nodes.find(key); // no nodes[key], silent create
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

    // purege expired entries then evict LRU if still over cap, T: O(1) avg, O(n) if purge runs
    void put(int key, int value, int ttl, long long now) {
        long long expireAt = ttl == 0 ? 0 : now + ttl;
        auto it = nodes.find(key);
        if (it != nodes.end()) { // update existing node
            it->second->val = value;
            it->second->expireAt = expireAt;
            remove(it->second);
            pushFront(it->second);
            return;
        }

        if ((int)nodes.size() >= cap) { // purge expired
            Node* cur = tail.prev;
            while (cur != &head) {
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
    check(c.get(2) == -1, "evict 2");
    c.put(4, 4);
    check(c.get(1) == -1, "evict 1");
    check(c.get(3) == 3 && c.get(4) == 4, "keep 3 4");
}
