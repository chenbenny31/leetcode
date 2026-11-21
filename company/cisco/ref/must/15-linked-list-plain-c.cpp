// 15 · Linked List In Plain C (Cisco live rounds, Easy) — reverse, delete, cycle
// the embedded rounds ask the list basics the C way: no containers, no extra allocation
// (1) reverse a singly linked list, iterative and recursive (LC 206)
// (2) delete the first node holding a given value
// (3) delete a node given only a pointer to it (LC 237)
// (4) does the list have a cycle (LC 141)
//
// clarify:
// head may be null -> yes, return it as is
// who owns the nodes -> they come from malloc, so delete must free
// (2) value not found -> list unchanged
// (3) node is never the tail -> given; w/ only the node the tail can't be unlinked
// list len -> recursion uses one stack frame per node, so iterative is the default
//
// in real C: `struct Node*` (or a typedef), NULL for nullptr, <stdbool.h> for bool,
// and malloc needs no cast

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;
using std::malloc;
using std::free;

struct Node {
    int val;
    Node* next;
};

// (1a) reverse, iterative
// core idea: walk once, point each node back at prev; prev ends as the new head
// T: O(n), S: O(1)
Node* reverseIter(Node* head) {
    Node* prev = nullptr;
    Node* cur = head;
    while (cur) {
        // save before overwriting cur->next
        Node* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    return prev;
}

// (1b) reverse, recursive
// core idea: reverse the rest, then hang head after its old next (the tail of the rest)
// T: O(n), S: O(n) — call stack
Node* reverseRec(Node* head) {
    if (!head || !head->next) { return head; }
    Node* newHead = reverseRec(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

// (2) delete the first node w/ val; returns the (maybe new) head
// core idea: the head has no previous node, so handle it first; then stop at the node
// before the match
// T: O(n), S: O(1)
Node* deleteValue(Node* head, int val) {
    if (!head) { return head; }
    if (head->val == val) {
        Node* rest = head->next;
        free(head);
        return rest;
    }
    Node* prev = head;
    while (prev->next && prev->next->val != val) { prev = prev->next; }
    if (prev->next) {
        Node* victim = prev->next;
        prev->next = victim->next;
        free(victim);
    }
    return head;
}

// (3) delete given only the node
// core idea: can't reach the previous node, so copy the next node here, unlink that one
// T: O(1), S: O(1)
void deleteGivenNode(Node* node) {
    Node* victim = node->next;
    node->val = victim->val;
    node->next = victim->next;
    free(victim);
}

// (4) cycle check
// core idea: fast moves 2, slow moves 1; inside a cycle fast catches slow
// T: O(n), S: O(1)
bool hasCycle(Node* head) {
    Node* slow = head;
    Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { return true; }
    }
    return false;
}

Node* build(const vector<int>& vals) {
    Node* head = nullptr;
    Node* tail = nullptr;
    for (int v : vals) {
        Node* node = (Node*)malloc(sizeof(Node));
        node->val = v;
        node->next = nullptr;
        if (!head) { head = node; } else { tail->next = node; }
        tail = node;
    }
    return head;
}

vector<int> toVec(Node* head) {
    vector<int> out;
    for (Node* cur = head; cur; cur = cur->next) { out.push_back(cur->val); }
    return out;
}

void freeList(Node* head) {
    while (head) {
        Node* nxt = head->next;
        free(head);
        head = nxt;
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Node* list = reverseIter(build({1, 2, 3}));
    check(toVec(list) == vector<int>({3, 2, 1}), "reverse-iter");
    list = reverseRec(list);
    check(toVec(list) == vector<int>({1, 2, 3}), "reverse-rec");
    check(reverseIter(nullptr) == nullptr, "reverse-empty");

    list = deleteValue(list, 1);
    check(toVec(list) == vector<int>({2, 3}), "delete-head");
    list = deleteValue(list, 9);
    check(toVec(list) == vector<int>({2, 3}), "delete-missing");
    deleteGivenNode(list);
    check(toVec(list) == vector<int>({3}), "delete-given-node");

    Node* tail = list;
    check(!hasCycle(list), "no-cycle");
    tail->next = list;
    check(hasCycle(list), "self-cycle");
    tail->next = nullptr;
    freeList(list);
    return 0;
}

// follow-up:
// delete w/o the head special case -> walk a Node** that points at the link to cur
// delete every node w/ val (LC 203) -> same walk, don't stop at the first match
// reverse only positions left..right (LC 92, reported) -> walk to the node before left,
//   then move each next node to the front of the sublist
// where the cycle starts (LC 142) -> select/06-linked-list/03-list-cycle-ii
// middle node -> slow / fast, slow is the middle when fast hits the end
// doubly linked list -> fix both next and prev; delete given the node is O(1) w/o copy
// binary list to int (LC 1290, reported in the OA) -> ref/recommended/05
/*
Node* deleteValue(Node* head, int val) {
    Node** link = &head;
    while (*link && (*link)->val != val) { link = &(*link)->next; }
    if (*link) {
        Node* victim = *link;
        *link = victim->next;
        free(victim);
    }
    return head;
}
*/
