// LC 622 Design Circular Queue (Medium). Pattern: ring buffer, head + count.
// Fixed capacity k. enQueue / deQueue return false when full / empty; Front / Rear
// return -1 when empty; isEmpty, isFull.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Array + head index + count; the tail slot is (head + count) % k. T: O(1) per op,
// S: O(k).
class MyCircularQueue {
    // circular storage; capacity = data.size()
    vector<int> data;
    // index of the front element
    int head = 0;
    // number of stored elements; tells full from empty when the tail wraps onto head
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
    test("empty-dequeue", q.deQueue(), false);
}

// follow-up: no count field -> head + tail with one slot left unused: empty iff
//   head == tail, full iff (tail + 1) % cap == head
// follow-up: thread-safe blocking queue -> mutex + condition_variables notFull /
//   notEmpty; wait in a while loop on the predicate, notify the other side
