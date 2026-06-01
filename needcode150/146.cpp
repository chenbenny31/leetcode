// hash-map + doubly-linked-list, T: O(1) get/put, S: O(cap)

#include <unordered_map>
#include <cstddef> // nullptr

class LRUCache {
private:
    struct Node {
        int key, val;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {} 
    };

    int capacity;
    std::unordered_map<int, Node*> map; // key: Node*
    Node* head; // dummy head, most recent side
    Node* tail; // dummy tail, least recent side

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insert_front(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

public:
    LRUCache(int capacity) : capacity(capacity) {
        map.reserve(capacity);
        map.max_load_factor(0.25f);
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* succ = curr->next;
            delete curr;
            curr = succ;
        }
    }

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) { return -1; }
        remove(it->second);
        insert_front(it->second);
        return it->second->val;
    }

    void put(int key, int val) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->val = val;
            remove(it->second);
            insert_front(it->second);
            return;
        }

        if (static_cast<int>(map.size()) == capacity) {
            Node* lru = tail->prev;
            remove(lru);
            map.erase(lru->key);
            delete lru;
        }
        Node* node = new Node(key, val);
        insert_front(node);
        map[key] = node;
    }
};

// sentinal node: head, tail embedded as class member (Node for stack alloc, Node* for heap alloc)
// necessity of doubly linked list: O(1) remove middle node need prev pointer
// pre-alloc map with capacity and factor
