// hahs-map, T: O(n), S: O(n)

#include <unordered_map>
#include <cstddef> // nullptr

class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) { return nullptr; }

        std::unordered_map<Node*, Node*> map; // orig-node: copy-node
        int count = 0;
        Node* curr = head;
        while (curr) {
            count++;
            curr = curr->next;
        }
        map.max_load_factor(0.25f);

        curr = head;
        while (curr) {
            map[curr] = new Node(curr->val);
            curr = curr->next;
        }

        curr = head;
        while (curr) {
            map[curr]->next = map[curr->next];
            map[curr]->random = map[curr->random];
            curr = curr->next;
        }

        return map[head];
    }
};

// interleaving, T: O(n), S: O(1)

#include <cstddef>

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
        Node* copy = &dummy;
        curr = head;
        while (curr) {
            copy->next = curr->next;
            copy = copy->next;
            curr->next = copy->next;
            curr = curr->next;
        }
        return dummy.next;
    }
};

// interleaving works for random: curr->random->next is the copy of curr->random
// map[nullptr]: std::unordered_map default-constructs missing keys - map[nullptr] -> nullptr
// new Node on hot path: prod fix use memory pool to pre-alloc slab of n nodes
// count and reserve with correct size: the first traversal warm cache and ensure no re-alloc

