// 19 · Bounded blocking queue — quant core (not reported)
// any num of producer and consumer threads; push waits while the queue is full, pop
// waits while it is empty; close() wakes every waiting thread so the system can shut down
//
// clarify:
// full queue -> blocks the producer (backpressure), nothing is dropped
// after close() -> push fails; pop still hands out what is left, then fails
// order -> FIFO, in the order the producers got the lock
// compile -> add -pthread

#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

using std::cout;
using std::string;
using std::queue;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::thread;

// core idea: one mutex guards the queue; one condition variable per reason to wait
// T: O(1) per op besides the waiting, S: O(capacity)
class BlockingQueue {
    queue<int> items;            // waiting msgs, oldest first
    int cap;
    bool closed = false;
    mutex mtx;                   // guards items and closed
    condition_variable notFull;  // producers wait here
    condition_variable notEmpty; // consumers wait here

public:
    explicit BlockingQueue(int capacity) : cap(capacity) {}

    // false if the queue is closed
    bool push(int val) {
        unique_lock<mutex> lock(mtx);
        // while, not if: a wakeup can be spurious, or another thread took the slot
        while ((int)items.size() == cap && !closed) { notFull.wait(lock); }
        if (closed) { return false; }
        items.push(val);
        notEmpty.notify_one();
        return true;
    }

    // false if the queue is closed and drained
    bool pop(int& val) {
        unique_lock<mutex> lock(mtx);
        while (items.empty() && !closed) { notEmpty.wait(lock); }
        if (items.empty()) { return false; }
        val = items.front();
        items.pop();
        notFull.notify_one();
        return true;
    }

    void close() {
        unique_lock<mutex> lock(mtx);
        closed = true;
        // notify_all: every waiting thread must wake up and see closed
        notFull.notify_all();
        notEmpty.notify_all();
    }
};

// producer thread: push 1 .. total, then close the queue
void produce(BlockingQueue* queue, int total) {
    for (int i = 1; i <= total; i++) { queue->push(i); }
    queue->close();
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    int total = 10000;
    BlockingQueue queue(4);
    thread producer(produce, &queue, total);

    bool inOrder = true;
    int count = 0;
    int val = 0;
    while (queue.pop(val)) {
        count++;
        if (val != count) { inOrder = false; }
    }
    producer.join();

    check(inOrder && count == total, "all-in-order");
    check(!queue.pop(val), "pop-after-close");
    check(!queue.push(1), "push-after-close");
}

// follow-up:
// 1. wait(lock, pred) -> the same while loop, written as a predicate
// 2. pop with a timeout -> wait_for; return false when it runs out
// 3. never block the producer -> tryPush returns false when full, or drop the oldest
// 4. one producer + one consumer, no lock -> two atomic counters (24-spsc-ring)
// 5. less lock traffic -> pop a batch: swap the whole queue out under the lock
// 6. why notify under the lock is fine -> the woken thread just waits for the mutex
