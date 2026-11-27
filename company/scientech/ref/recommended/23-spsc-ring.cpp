// 23 · SPSC ring buffer — resume drill (explain it; typing it is a follow-up)
// exactly one producer thread and one consumer thread, no lock
// push(val) -> false when full, pop(val&) -> false when empty; neither one waits
//
// clarify:
// threads -> one calls push, one other calls pop (more consumers -> 25-spmc-ring)
// capacity -> fixed at construction; a msg is a small value that is cheap to copy
// full / empty -> return false right away, the caller retries or drops
// compile -> add -pthread

#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <cstdint>

using std::cout;
using std::string;
using std::vector;
using std::atomic;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;
using std::thread;
using std::this_thread::yield;
using std::int64_t;
using std::uint64_t;

// core idea: two counters, one writer each (producer -> writePos, consumer -> readPos);
// a slot is written before writePos moves past it and read before readPos moves past it
// release store + acquire load: whoever sees the new counter also sees the slot's data
// T: O(1) per op, S: O(capacity)
class SpscRing {
    vector<int64_t> slots;     // slots[n % cap] = msg n
    uint64_t cap;
    atomic<uint64_t> readPos;  // num of msgs popped, written by the consumer only
    atomic<uint64_t> writePos; // num of msgs pushed, written by the producer only

public:
    SpscRing(int capacity) : slots(capacity), cap(capacity), readPos(0), writePos(0) {}

    // producer only
    bool push(int64_t val) {
        uint64_t nwrite = writePos.load(memory_order_relaxed); // my own counter
        uint64_t nread = readPos.load(memory_order_acquire);
        if (nwrite - nread == cap) { return false; }
        slots[nwrite % cap] = val;
        // publish only after the slot is set
        writePos.store(nwrite + 1, memory_order_release);
        return true;
    }

    // consumer only
    bool pop(int64_t& val) {
        uint64_t nread = readPos.load(memory_order_relaxed); // my own counter
        uint64_t nwrite = writePos.load(memory_order_acquire);
        if (nread == nwrite) { return false; }
        val = slots[nread % cap];
        // free the slot only after it is read
        readPos.store(nread + 1, memory_order_release);
        return true;
    }
};

// producer thread: push 0 .. total - 1, retry while the ring is full
void produce(SpscRing* ring, int total) {
    for (int i = 0; i < total; i++) {
        while (!ring->push(i)) { yield(); }
    }
}

// consumer side: pop total msgs, they must come out as 0, 1, 2, ...
bool consumeInOrder(SpscRing* ring, int total) {
    bool inOrder = true;
    for (int want = 0; want < total; want++) {
        int64_t val = 0;
        while (!ring->pop(val)) { yield(); }
        if (val != want) { inOrder = false; }
    }
    return inOrder;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SpscRing ring(2);
    int64_t val = 0;
    check(!ring.pop(val), "empty");
    check(ring.push(10) && ring.push(11) && !ring.push(12), "full");
    check(ring.pop(val) && val == 10, "fifo");
    check(ring.push(12) && ring.pop(val) && val == 11, "wrap");
    check(ring.pop(val) && val == 12 && !ring.pop(val), "empty-again");

    SpscRing shared(4);
    thread producer(produce, &shared, 100000);
    bool inOrder = consumeInOrder(&shared, 100000);
    producer.join();
    check(inOrder, "2-threads-in-order");
}

// follow-up:
// 1. why no lock -> each counter has one writer; the other side only reads it
// 2. false sharing -> alignas(64) on readPos and writePos, one cache line each
// 3. fewer atomic loads -> cache the other side's counter; reload when it looks full
// 4. no % in the hot path -> power-of-two capacity, idx = n & (cap - 1)
// 5. more consumers -> CAS on head, or a seqlock per slot for broadcast (25-spmc-ring)
// 6. wait instead of spin -> mutex + condition variable (18-blocking-queue)
