// ByteDance (hack2hire): implement a FIFO queue from scratch with a singly linked list:
// enqueue, dequeue (returns the front value), front, isEmpty; empty dequeue/front throw.
// Follow-up: circular-array queue that doubles its capacity when full.
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using std::cout;
using std::string;
using std::vector;
using std::out_of_range;

// Linked list with head (dequeue end) and tail (enqueue end). T: O(1) per op, S: O(n).
class Queue {
    struct Node {
        int val;
        Node* next;
    };
    // front: dequeue end
    Node* head = nullptr;
    // back: enqueue end
    Node* tail = nullptr;
    // number of nodes
    int size = 0;

public:
    Queue() = default;

    ~Queue() {
        while (head) {
            Node* nxt = head->next;
            delete head;
            head = nxt;
        }
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void enqueue(int x) {
        Node* node = new Node{x, nullptr};
        if (tail) {
            tail->next = node;
        } else {
            head = node;
        }
        tail = node;
        size++;
    }

    int dequeue() {
        if (!head) { throw out_of_range("dequeue on empty queue"); }
        Node* node = head;
        int val = node->val;
        head = head->next;
        // removed the last node: tail must not dangle
        if (!head) { tail = nullptr; }
        delete node;
        size--;
        return val;
    }

    int front() {
        if (!head) { throw out_of_range("front on empty queue"); }
        return head->val;
    }

    bool isEmpty() { return size == 0; }
};

// follow-up: ring buffer, doubled when full. T: O(1) amortized enqueue, S: O(n).
class CircularQueue {
    // ring buffer; capacity = data.size()
    vector<int> data;
    // index of the front element
    int head = 0;
    // number of elements stored
    int size = 0;

    // copy into a buffer twice as large, front moved to index 0
    void grow() {
        int cap = data.size();
        vector<int> bigger(2 * cap);
        for (int i = 0; i < size; i++) { bigger[i] = data[(head + i) % cap]; }
        data = bigger;
        head = 0;
    }

public:
    CircularQueue() : data(4) {}

    void enqueue(int x) {
        if (size == (int)data.size()) { grow(); }
        data[(head + size) % data.size()] = x;
        size++;
    }

    int dequeue() {
        if (size == 0) { throw out_of_range("dequeue on empty queue"); }
        int val = data[head];
        head = (head + 1) % data.size();
        size--;
        return val;
    }

    int front() {
        if (size == 0) { throw out_of_range("front on empty queue"); }
        return data[head];
    }

    bool isEmpty() { return size == 0; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Queue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    check(q.front() == 1 && q.dequeue() == 1 && q.dequeue() == 2 && q.dequeue() == 3,
          "fifo");
    q.enqueue(4);
    check(q.front() == 4 && q.dequeue() == 4 && q.isEmpty(), "reuse-after-empty");
    bool threw = false;
    try {
        q.dequeue();
    } catch (const out_of_range&) {
        threw = true;
    }
    check(threw, "empty-throws");

    CircularQueue cq;
    for (int i = 1; i <= 3; i++) { cq.enqueue(i); }
    cq.dequeue();
    cq.dequeue();
    for (int i = 4; i <= 8; i++) { cq.enqueue(i); }
    bool inOrder = true;
    for (int want = 3; want <= 8; want++) {
        int got = cq.dequeue();
        if (got != want) { inOrder = false; }
    }
    check(inOrder && cq.isEmpty(), "ring-wrap-and-grow");
}

// follow-up: growable ring buffer -> see CircularQueue above; double if full, O(1) amort.
// follow-up: queue from two stacks (LC 232) -> push to in; pop from out, refill out from
//   in only when out is empty; each element moved once, O(1) amortized
// follow-up: thread-safe / blocking -> mutex + condition_variable notEmpty, wait in a
//   while loop in dequeue; bounded -> add notFull (LC 1188)
