// blocking queue: bounded, any num of producer and consumer threads, with a lock
// push waits while the queue is full, pop waits while it is empty
// close() wakes every waiting thread so the system can shut down

// usage:
// g++ -std=c++17 -Wall -Wextra -pthread 07-blocking-queue.cpp -o a && ./a
//
// BlockingQueue queue(1024);  queue.push(val);  queue.pop(val);  queue.close();

// clarify / assumptions
// waiting is wanted here (e.g. a logging or risk thread); no-wait versions -> 16 / 17
// after close(): push fails, pop still hands out what is left, then fails
// FIFO across all producers, in the order they got the lock

// data:
// items: waiting msgs, oldest first
// mtx guards items and closed; one condition variable per reason to wait

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <iostream>

using std::condition_variable;
using std::int64_t;
using std::size_t;
using std::cout;
using std::lock_guard;
using std::mutex;
using std::queue;
using std::string;
using std::thread;
using std::unique_lock;

class BlockingQueue {
private:
    queue<int64_t> items; // waiting msgs, oldest first
    size_t cap;
    bool closed;
    mutex mtx; // guards items and closed
    condition_variable notFull; // producers wait here
    condition_variable notEmpty; // consumers wait here
public:
    BlockingQueue(int capacity) : cap(capacity), closed(false) {}

    // wait while full; return false if the queue is closed, T: O(1) + waiting
    bool push(int64_t val) {
        unique_lock<mutex> lock(mtx);
        while (items.size() == cap && !closed) { // loop: a wakeup can be spurious
            notFull.wait(lock); // gives up mtx while asleep, holds it again after
        }
        if (closed) { return false; }

        items.push(val);
        notEmpty.notify_one();
        return true;
    }

    // wait while empty; return false if closed and nothing is left
    bool pop(int64_t& val) {
        unique_lock<mutex> lock(mtx);
        while (items.empty() && !closed) {
            notEmpty.wait(lock);
        }
        if (items.empty()) { return false; } // closed and drained

        val = items.front();
        items.pop();
        notFull.notify_one();
        return true;
    }

    void close() {
        lock_guard<mutex> lock(mtx);
        closed = true;
        notFull.notify_all(); // everyone must wake up and see closed
        notEmpty.notify_all();
    }
};

// producer thread: push 1 .. total, then close the queue
void produce(BlockingQueue* queue, int total) {
    for (int i = 1; i <= total; i++) {
        queue->push(i);
    }
    queue->close();
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // a small queue forces the producer to wait on full, the consumer on empty
    int total = 10000;
    BlockingQueue queue(4);
    thread producer(produce, &queue, total);

    bool inOrder = true;
    int count = 0;
    int64_t val = 0;
    bool popped = queue.pop(val);
    while (popped) {
        count++;
        if (val != count) { inOrder = false; }
        popped = queue.pop(val);
    }
    producer.join();

    check("all-in-order", inOrder && count == total);
    check("pop-after-close", !queue.pop(val));
    check("push-after-close", !queue.push(1));
}
