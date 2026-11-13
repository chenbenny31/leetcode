// LC 622 Design Circular Queue (Medium) — ring buffer, head + count
// Fixed capacity k. enQueue / deQueue return false when full / empty; Front / Rear
// return -1 when empty; isEmpty, isFull.
// Clarify: k >= 1; values non-negative so -1 is a safe "empty" answer;
//   single-threaded.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: array + head index + count; the tail slot is (head + count) % k, and
// count tells full from empty. T: O(1) per op, S: O(k).
class MyCircularQueue {
    // circular storage; capacity = data.size()
    vector<int> data;
    // index of the front element
    int head = 0;
    // number of stored elements
    int count = 0;

public:
    MyCircularQueue(int k) : data(k) {}

    bool enQueue(int value) {
        if (isFull()) { return false; }
        // first free slot after the last element
        int tail = (head + count) % (int)data.size();
        data[tail] = value;
        count++;
        return true;
    }

    bool deQueue() {
        if (isEmpty()) { return false; }
        head = (head + 1) % (int)data.size();
        count--;
        return true;
    }

    int Front() { return isEmpty() ? -1 : data[head]; }

    int Rear() {
        if (isEmpty()) { return -1; }
        int last = (head + count - 1) % (int)data.size();
        return data[last];
    }

    bool isEmpty() { return count == 0; }

    bool isFull() { return count == (int)data.size(); }
};

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    MyCircularQueue q(2);
    q.enQueue(1);
    q.enQueue(2);
    test("reject-full", q.enQueue(3), false);
    q.deQueue();
    q.enQueue(3);
    test("wrap-front", q.Front(), 2);
    test("wrap-rear", q.Rear(), 3);
    q.deQueue();
    q.deQueue();
    test("empty-front", q.Front(), -1);
    return 0;
}

// follow-up (ideas only):
// - no count field -> head + tail with one slot left unused: empty iff head == tail,
//   full iff (tail + 1) % cap == head
// - thread-safe blocking queue -> mutex + condition variables notFull / notEmpty; wait
//   in a loop on the predicate, notify the other side after each change
