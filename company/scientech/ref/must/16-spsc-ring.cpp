// SPSC ring buffer: exactly one producer thread and one consumer thread, no lock
// push(val) -> false when full, pop(val&) -> false when empty; neither one waits

// usage:
// g++ -std=c++17 -Wall -Wextra -pthread 16-spsc-ring.cpp -o a && ./a
//
// SpscRing ring(1024);  producer: ring.push(val);  consumer: ring.pop(val);

// clarify / assumptions
// one thread calls push, one other thread calls pop (more threads -> 17-spmc-ring)
// capacity is fixed at construction; a msg is a small value that is cheap to copy
// full / empty -> return false right away, the caller retries or drops

// data:
// writePos: num of msgs pushed so far, only the producer changes it
// readPos:  num of msgs popped so far, only the consumer changes it
// msg n lives in slots[n % cap]; num of msgs waiting = writePos - readPos

// why no lock is needed:
// every variable has one writer: producer -> writePos, consumer -> readPos
// a slot is written before writePos moves past it, and read before readPos moves past it
// release store + acquire load: whoever sees the new counter also sees the slot's data

#include <atomic>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

using std::atomic;
using std::int64_t;
using std::uint64_t;
using std::cout;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;
using std::string;
using std::thread;
using std::vector;
using std::this_thread::yield;

class SpscRing {
private:
    vector<int64_t> slots; // slots[n % cap] = msg n
    uint64_t cap;
    atomic<uint64_t> readPos; // num of msgs popped, written by the consumer only
    atomic<uint64_t> writePos; // num of msgs pushed, written by the producer only
public:
    SpscRing(int capacity) : slots(capacity), cap(capacity), readPos(0), writePos(0) {}

    // producer only, T: O(1)
    bool push(int64_t val) {
        uint64_t write = writePos.load(memory_order_relaxed); // my own counter
        uint64_t read = readPos.load(memory_order_acquire);
        if (write - read == cap) { return false; } // full

        slots[write % cap] = val;
        writePos.store(write + 1, memory_order_release); // publish after the slot is set
        return true;
    }

    // consumer only, T: O(1)
    bool pop(int64_t& val) {
        uint64_t read = readPos.load(memory_order_relaxed); // my own counter
        uint64_t write = writePos.load(memory_order_acquire);
        if (read == write) { return false; } // empty

        val = slots[read % cap];
        readPos.store(read + 1, memory_order_release); // free the slot after it is read
        return true;
    }
};

// producer thread: push 0 .. total - 1, retry while the ring is full
void produce(SpscRing* ring, int total) {
    for (int i = 0; i < total; i++) {
        bool pushed = ring->push(i);
        while (!pushed) {
            yield();
            pushed = ring->push(i);
        }
    }
}

// consumer side: pop total msgs, they must come out as 0, 1, 2, ...
bool consumeInOrder(SpscRing* ring, int total) {
    bool inOrder = true;
    for (int want = 0; want < total; want++) {
        int64_t val = 0;
        bool popped = ring->pop(val);
        while (!popped) {
            yield();
            popped = ring->pop(val);
        }
        if (val != want) { inOrder = false; }
    }
    return inOrder;
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test on one thread
    SpscRing ring(2);
    int64_t val = 0;
    check("empty", !ring.pop(val));
    check("push", ring.push(10) && ring.push(11));
    check("full", !ring.push(12));
    check("fifo", ring.pop(val) && val == 10);
    bool wrapped = (ring.push(12) && ring.pop(val) && val == 11);
    check("wrap", wrapped && ring.pop(val) && val == 12);
    check("empty-again", !ring.pop(val));

    // test on two threads: a small ring forces many full / empty retries
    SpscRing shared(4);
    thread producer(produce, &shared, 100000);
    bool inOrder = consumeInOrder(&shared, 100000);
    producer.join();
    check("2-threads-in-order", inOrder);
}
