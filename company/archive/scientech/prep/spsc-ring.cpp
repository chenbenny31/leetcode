// one producer and one consumer thread, no lock
// push(val) -> false when full, pop(val&) -> false when empty, neither waits

// one thread call push, one other calls pop
// cap is fixed at construct; msg is small value allow copy
// full/empty return false right away

// data:
// writePos: num of msgs pushed so far
// readPos: num of msgs popped so far
// msg n lives in slots[n % cap], num of msgs waiting = writePos - readPos

// no lock is needed:
// msg binding one writer and one reader, slot is written then read, release store + acquire load

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
using std::memory_order_release;
using std::memory_order_relaxed;
using std::string;
using std::vector;
using std::thread;
using std::this_thread::yield;

class SpscRing {
private:
    vector<int64_t> slots; // slots[n % cap] = msg-nth
    uint64_t cap;
    atomic<uint64_t> readPos; // num of msgs poped
    atomic<uint64_t> writePos; // num of msgs pushed

public:
    SpscRing(int cap) : slots(cap), cap(cap), readPos(0), writePos(0) {}

    bool push(int64_t val) {
        uint64_t nwrite = writePos.load(memory_order_relaxed);
        uint64_t nread = readPos.load(memory_order_acquire);
        if (nwrite - nread == cap) { return false; } // full

        slots[nwrite % cap] = val;
        writePos.store(nwrite + 1, memory_order_release); // publish after slot is set
        return true;
    }

    bool pop(int64_t& val) {
        uint64_t nread = readPos.load(memory_order_relaxed);
        uint64_t nwrite = writePos.load(memory_order_acquire);
        if (nread == nwrite) { return false; } // empty

        val = slots[nread % cap];
        readPos.store(nread + 1, memory_order_release);
        return true;
    }
};

// producer thread, push 0..total-1, retry when full
void produce(SpscRing* ring, int total) {
    for (int i = 0; i < total; i++) {
        bool pushed = ring->push(i);
        while (!pushed) {
            yield();
            pushed = ring->push(i);
        }
    }
}

// consumer thread: pop total msgs, need come out as 0,1,2..order
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

