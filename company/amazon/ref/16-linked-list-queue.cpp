// Generic FIFO queue built from linked-list nodes (no library containers): enqueue(x) and
// dequeue() in O(1).
// Follow-ups: remove(x) deletes the first occurrence of x; removeAllDuplicates() keeps
// only the first occurrence of each value, in order.

#include <iostream>
#include <string>

using std::cout;
using std::string;

template <typename T>
class LinkedQueue {
    struct Node {
        T val;
        Node* next;
    };
    // oldest node (dequeue end) and newest node (enqueue end)
    Node* head = nullptr;
    Node* tail = nullptr;

public:
    ~LinkedQueue() {
        while (head) {
            Node* nxt = head->next;
            delete head;
            head = nxt;
        }
    }

    bool empty() const { return head == nullptr; }

    // T: O(1)
    void enqueue(const T& x) {
        Node* node = new Node{x, nullptr};
        if (tail) {
            tail->next = node;
        } else {
            head = node;
        }
        tail = node;
    }

    // precondition: not empty. T: O(1)
    T dequeue() {
        Node* old = head;
        T x = old->val;
        head = old->next;
        if (!head) { tail = nullptr; }
        delete old;
        return x;
    }

    // follow-up: unlink the first node holding x. T: O(n)
    bool remove(const T& x) {
        Node* prev = nullptr;
        for (Node* cur = head; cur; cur = cur->next) {
            if (cur->val != x) {
                prev = cur;
                continue;
            }
            if (prev) {
                prev->next = cur->next;
            } else {
                head = cur->next;
            }
            if (cur == tail) { tail = prev; }
            delete cur;
            return true;
        }
        return false;
    }

    // follow-up: for each kept node, unlink later nodes with the same value. Without a
    // hash set this is T: O(n^2), S: O(1).
    void removeAllDuplicates() {
        for (Node* keep = head; keep; keep = keep->next) {
            Node* prev = keep;
            Node* cur = keep->next;
            while (cur) {
                if (cur->val == keep->val) {
                    prev->next = cur->next;
                    if (cur == tail) { tail = prev; }
                    delete cur;
                    cur = prev->next;
                } else {
                    prev = cur;
                    cur = cur->next;
                }
            }
        }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    LinkedQueue<int> q;
    check(q.empty(), "empty");
    q.enqueue(1);
    q.enqueue(2);
    check(q.dequeue() == 1, "fifo");
    check(q.dequeue() == 2 && q.empty(), "drain");
    q.enqueue(3);
    check(q.dequeue() == 3, "reuse");
    LinkedQueue<string> words;
    words.enqueue("a");
    words.enqueue("b");
    check(words.dequeue() == "a", "generic");
}
