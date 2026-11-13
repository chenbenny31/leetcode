// LC 622 Design Circular Queue (Medium) — ring buffer, head + count
// fixed capacity k; enQueue / deQueue return false when full / empty
// Front / Rear return -1 when empty; isEmpty, isFull
//
// clarify:
// k >= 1 -> % data.size() is safe
// values >= 0 -> -1 is a safe "empty" answer
// single-threaded -> no lock; blocking queue -> follow-up

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: array + head idx + count; tail slot = (head + count) % k
// count tells full from empty
// T: O(1) per op, S: O(k)
class MyCircularQueue {
    // circular storage; capacity = data.size()
    vector<int> data;
    // idx of the front element
    int head = 0;
    // num of stored elements
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

// follow-up:
// no count field -> one unused slot: empty iff head == tail, full iff next(tail) == head
// thread-safe blocking queue -> mutex + cond vars notFull / notEmpty; wait in a loop
