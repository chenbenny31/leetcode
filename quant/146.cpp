// doubly-linkedlist + hash-map

#include <unordered_map>
#include <cstddef>

class LRUCache {
private:
    struct Node {
        int key;
        int val;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;
    Node* head;
    Node* tail;
    std::unordered_map<int, Node*> map; // key: node

    void addFront(Node* node) {
        node->next = head->next;
        node->prev = head;
        node->next->prev = node;
        head->next = node;
    }

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

public:
    LRUCache(int capacity) : cap(capacity) {
        map.reserve(cap);
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        auto it = map.find(key); 
        if (it == map.end()) { return -1; }
        Node* node = it->second;
        remove(node);
        addFront(node);
        return node->val;
    }
    
    void put(int key, int value) {
        auto it = map.find(key); 
        if (it != map.end()) {
            Node* node = it->second;
            node->val = value;
            remove(node);
            addFront(node);
            return;
        }

        if (static_cast<int>(map.size()) >= cap) {
            Node* lru = tail->prev;
            remove(lru);
            map.erase(lru->key);
            delete lru;
        }

        Node* newNode = new Node(key, value);
        addFront(newNode);
        map[key] = newNode;
    }

    ~LRUCache() {
        Node* curr = head;
        while (curr) {
            Node* succ = curr->next;
            delete curr;
            curr = succ;
        }
    }
};

// std::list + iterator

#include <unordered_map>
#include <list>
#include <utility> // std::pair

class LRUCache {
private:
    int cap;
    std::list<std::pair<int, int>> items; // rear = lru
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> map;

public:
    LRUCache(int capacity) : cap(capacity) {
        map.reserve(cap);
    }

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) { return -1; }
        items.splice(items.begin(), items, it->second); // remove it->second from items + re-insert at items.begin()
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }

        if (static_cast<int>(map.size()) >= cap) {
            map.erase(items.back().first);
            items.pop_back();
        }
        items.emplace_front(key, value);
        map[key] = items.begin();
    }
};
