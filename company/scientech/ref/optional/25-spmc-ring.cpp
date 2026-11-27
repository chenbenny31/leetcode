// 25 · SPMC ring — roster project drill (explain it; beyond a new-grad coding round)
// one producer thread, many consumer threads, no lock
// (1) work queue: each msg is taken by exactly one consumer (orders -> worker threads)
// (2) broadcast: every consumer sees every msg (market data -> many strategies); the
//     producer never waits, so a slow consumer can be lapped and must notice it
//
// clarify:
// first question -> does a msg go to one consumer (1) or to all of them (2)?
// producer -> one thread; capacity > 0, fixed at construction
// counters and per-slot seq nums -> never wrap
// (2) lapped consumer -> the data is lost for it; it resyncs to the newest msg
// atomic ops -> default (seq_cst) order: correct first, relaxing is a follow-up
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
using std::thread;
using std::this_thread::yield;
using std::int64_t;
using std::uint64_t;

// ==================== (1) ====================
// each msg is handed to one consumer; a full queue rejects the write

// core idea: consumers claim msg n by moving head from n to n + 1 with CAS, so only
// one of them can win it
// T: push O(1), pop O(1) + retries lost to other consumers, S: O(capacity)
class SpmcQueue {
    // atomic slots: a consumer that loses the race may load a slot being rewritten
    vector<atomic<int64_t>> slots;
    uint64_t cap;
    atomic<uint64_t> head; // next msg to take, consumers race on it
    atomic<uint64_t> tail; // next msg to write, written by the producer only

public:
    SpmcQueue(int capacity) : slots(capacity), cap(capacity), head(0), tail(0) {}

    // producer only, false when full
    bool push(int64_t val) {
        uint64_t writePos = tail.load();
        if (writePos - head.load() == cap) { return false; }
        slots[writePos % cap].store(val);
        // publish only after the slot is set
        tail.store(writePos + 1);
        return true;
    }

    // any consumer, false when empty
    bool pop(int64_t& val) {
        uint64_t readPos = head.load();
        while (true) {
            if (readPos == tail.load()) { return false; }
            int64_t got = slots[readPos % cap].load();
            // a failed CAS reloads readPos with the cur head -> try the next msg
            if (head.compare_exchange_weak(readPos, readPos + 1)) {
                val = got;
                return true;
            }
        }
    }
};

// ==================== (2) ====================
// every consumer has its own cursor; an overwritten msg must be detected

struct Slot {
    atomic<uint64_t> seq{0}; // 2n + 1 = msg n is being written, 2n + 2 = msg n is ready
    atomic<int64_t> price{0};
    atomic<int64_t> qty{0};
};

// core idea: seqlock per slot; seq is odd while the producer writes it, and a reader
// keeps the fields only if seq was the same ready value before and after reading them
// T: O(1) per op, S: O(capacity)
class BroadcastRing {
    vector<Slot> slots;         // msg n lives in slots[n % cap] until msg n + cap
    uint64_t cap;
    atomic<uint64_t> published; // num of msgs published = number of the next msg

public:
    BroadcastRing(int capacity) : slots(capacity), cap(capacity), published(0) {}

    // producer only, never waits: overwrites the oldest slot
    void publish(int64_t price, int64_t qty) {
        uint64_t msg = published.load();
        Slot& slot = slots[msg % cap];
        slot.seq.store(2 * msg + 1);
        slot.price.store(price);
        slot.qty.store(qty);
        slot.seq.store(2 * msg + 2);
        published.store(msg + 1);
    }

    // read msg number readPos; each consumer owns its readPos
    // 1 = got it and readPos moved on, 0 = not published yet, -1 = lapped
    int read(uint64_t& readPos, int64_t& price, int64_t& qty) {
        Slot& slot = slots[readPos % cap];
        uint64_t want = 2 * readPos + 2; // seq of this slot while it holds msg readPos

        uint64_t before = slot.seq.load();
        if (before < want) { return 0; }  // an older msg, or still being written
        if (before > want) { return -1; } // a newer msg already took the slot

        int64_t gotPrice = slot.price.load();
        int64_t gotQty = slot.qty.load();
        if (slot.seq.load() != before) { return -1; } // overwritten while reading

        price = gotPrice;
        qty = gotQty;
        readPos++;
        return 1;
    }

    // a lapped consumer restarts from here (- 1 = the newest msg)
    uint64_t numPublished() { return published.load(); }
};

// ==================== tests ====================
// consumer thread of (1): take msgs until all are taken, add up what was taken
void takeAll(SpmcQueue* queue, int total, atomic<int>* taken, atomic<int64_t>* sum) {
    while (taken->load() < total) {
        int64_t val = 0;
        if (queue->pop(val)) {
            sum->fetch_add(val);
            taken->fetch_add(1);
        } else {
            yield();
        }
    }
}

// consumer thread of (2): follow the ring up to the last msg; every msg it reads must
// be whole (qty == 2 * price) and newer than the one before
void follow(BroadcastRing* ring, int64_t lastPrice, bool* ok) {
    uint64_t readPos = 0;
    int64_t prevPrice = -1;
    while (prevPrice != lastPrice) {
        int64_t price = 0;
        int64_t qty = 0;
        int got = ring->read(readPos, price, qty);
        if (got == 1) {
            if (qty != 2 * price || price <= prevPrice) { *ok = false; }
            prevPrice = price;
        } else if (got == 0) {
            yield();
        } else {
            readPos = ring->numPublished() - 1;
        }
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SpmcQueue queue(2);
    int64_t val = 0;
    check(!queue.pop(val), "queue-empty");
    check(queue.push(10) && queue.push(11) && !queue.push(12), "queue-full");
    check(queue.pop(val) && val == 10 && queue.pop(val) && val == 11, "queue-fifo");

    int total = 100000;
    SpmcQueue jobs(4);
    atomic<int> taken(0);
    atomic<int64_t> sum(0);
    thread worker1(takeAll, &jobs, total, &taken, &sum);
    thread worker2(takeAll, &jobs, total, &taken, &sum);
    for (int i = 1; i <= total; i++) {
        while (!jobs.push(i)) { yield(); }
    }
    worker1.join();
    worker2.join();
    check(taken.load() == total && sum.load() == 5000050000LL, "queue-each-once");

    BroadcastRing ring(2);
    uint64_t readPos = 0;
    int64_t price = 0;
    int64_t qty = 0;
    check(ring.read(readPos, price, qty) == 0, "ring-not-yet");
    ring.publish(100, 5);
    check(ring.read(readPos, price, qty) == 1 && price == 100 && qty == 5, "ring-read");

    uint64_t slowPos = 0;
    ring.publish(101, 6);
    ring.publish(102, 7);
    check(ring.read(slowPos, price, qty) == -1, "ring-lapped");
    slowPos = ring.numPublished() - 1;
    check(ring.read(slowPos, price, qty) == 1 && price == 102, "ring-resync");

    BroadcastRing feed(8);
    bool ok1 = true;
    bool ok2 = true;
    thread strat1(follow, &feed, total - 1, &ok1);
    thread strat2(follow, &feed, total - 1, &ok2);
    for (int i = 0; i < total; i++) { feed.publish(i, 2 * i); }
    strat1.join();
    strat2.join();
    check(ok1 && ok2, "ring-no-torn-read");
}

// follow-up:
// 1. relax the orders -> release on publish stores, acquire on the loads that pair up
// 2. (1) many producers too -> MPMC: a seq per slot (Vyukov queue), CAS on both ends
// 3. (2) slow consumer must not lose data -> producer waits on the min reader pos
// 4. false sharing -> alignas(64) on head, tail and each slot
// 5. wider msgs -> a plain struct copied between the two seq reads (the seqlock rule)
// 6. one consumer only -> drop the CAS: two counters (23-spsc-ring)
