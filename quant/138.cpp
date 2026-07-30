// hash-map

#include <unordered_map>
#include <cstddef>

struct Node {
    int val;
    Node* next;
    Node* random;
    Node(int val) : val(val), next(nullptr), random(nullptr) {}
};

class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) { return nullptr; }
        std::unordered_map<Node*, Node*> map;

        Node* curr = head;
        while (curr) {
            map[curr] = new Node(curr->val);
            curr = curr->next;
        }

        curr = head;
        while (curr) {
            map[curr]->next = curr->next ? map[curr->next] : nullptr;
            map[curr]->random = curr->random ? map[curr->random] : nullptr;
            curr = curr->next;
        }
        return map[head];
    }
};

// interleaving

class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) { return nullptr; }

        Node* curr = head;
        while (curr) {
            Node* copy = new Node(curr->val);
            copy->next = curr->next;
            curr->next = copy;
            curr = copy->next;
        }

        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }

        Node dummy(0);
        Node* tail = &dummy;
        curr = head;
        while (curr) {
            tail->next = curr->next;
            tail = tail->next;
            curr->next = tail->next;
            curr = curr->next;
        }
        return dummy.next;
    }
};
