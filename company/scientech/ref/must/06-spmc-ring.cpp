// SPMC ring: one producer thread, many consumer threads, no lock
// (1) work queue: each msg is taken by exactly one consumer (orders -> worker threads)
// (2) broadcast: every consumer sees every msg (market data -> many strategies);
//     the producer never waits, so a slow consumer can be lapped and must notice it

// usage:
// g++ -std=c++17 -Wall -Wextra -pthread 06-spmc-ring.cpp -o a && ./a
//
// (1) SpmcQueue queue(1024);     producer: queue.push(val);  consumer: queue.pop(val);
// (2) BroadcastRing ring(1024);  producer: ring.publish(price, qty);
//     consumer keeps its own readPos:  ring.read(readPos, price, qty);

// clarify / assumptions
// first question: does a msg go to one consumer (1) or to all of them (2)?
// one producer thread only; capacity fixed at construction
// (2) a msg is a quote {price, qty}; lapped consumer = data lost for it, it resyncs
// every atomic op uses the default (seq_cst) order: correct first, relax as a follow-up

// why each design is safe:
// (1) consumers claim msg n by moving head from n to n + 1 with CAS: only one can win
// (2) seqlock per slot: seq is odd while the producer writes it; a reader takes the
//     fields only if seq was the same ready value before and after reading them

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
using std::string;
using std::thread;
using std::vector;
using std::this_thread::yield;

// (1) work queue
class SpmcQueue {
private:
    // atomic slots: a consumer that loses the race may load a slot being rewritten
    vector<atomic<int64_t>> slots;
    uint64_t cap;
    atomic<uint64_t> head; // next msg to take, consumers race on it
    atomic<uint64_t> tail; // next msg to write, written by the producer only
public:
    SpmcQueue(int capacity) : slots(capacity), cap(capacity), head(0), tail(0) {}

    // producer only, false when full, T: O(1)
    bool push(int64_t val) {
        uint64_t writePos = tail.load();
        if (writePos - head.load() == cap) { return false; }

        slots[writePos % cap].store(val);
        tail.store(writePos + 1); // publish after the slot is set
        return true;
    }

    // any consumer, false when empty, T: O(1) + retries lost to other consumers
    bool pop(int64_t& val) {
        uint64_t readPos = head.load();
        while (true) {
            if (readPos == tail.load()) { return false; }

            int64_t got = slots[readPos % cap].load();
            // claim msg readPos; if another consumer was faster, CAS fails and
            // reloads readPos with the current head -> try the next msg
            if (head.compare_exchange_weak(readPos, readPos + 1)) {
                val = got;
                return true;
            }
        }
    }
};

// (2) broadcast
struct Slot {
    // 2 * n + 1 = msg n is being written, 2 * n + 2 = msg n is ready
    atomic<uint64_t> seq{0};
    atomic<int64_t> price{0};
    atomic<int64_t> qty{0};
};

class BroadcastRing {
private:
    vector<Slot> slots; // msg n lives in slots[n % cap] until msg n + cap overwrites it
    uint64_t cap;
    atomic<uint64_t> published; // num of msgs published = number of the next msg
public:
    BroadcastRing(int capacity) : slots(capacity), cap(capacity), published(0) {}

    // producer only, never waits: overwrites the oldest slot, T: O(1)
    void publish(int64_t price, int64_t qty) {
        uint64_t msg = published.load();
        Slot& slot = slots[msg % cap];

        slot.seq.store(2 * msg + 1); // odd: fields are changing
        slot.price.store(price);
        slot.qty.store(qty);
        slot.seq.store(2 * msg + 2); // even: msg is whole
        published.store(msg + 1);
    }

    // read msg number readPos; each consumer owns its readPos
    // return 1 = got it and readPos moved on, 0 = not published yet, -1 = lapped
    int read(uint64_t& readPos, int64_t& price, int64_t& qty) {
        Slot& slot = slots[readPos % cap];
        uint64_t want = 2 * readPos + 2; // seq of this slot when it holds msg readPos

        uint64_t before = slot.seq.load();
        if (before < want) { return 0; } // holds an older msg, or is being written
        if (before > want) { return -1; } // a newer msg already took the slot

        int64_t gotPrice = slot.price.load();
        int64_t gotQty = slot.qty.load();
        uint64_t after = slot.seq.load();
        if (after != before) { return -1; } // overwritten while reading: drop it

        price = gotPrice;
        qty = gotQty;
        readPos++;
        return 1;
    }

    // a lapped consumer restarts from here (- 1 = the newest msg)
    uint64_t numPublished() {
        return published.load();
    }
};

// consumer thread of (1): take msgs until all are taken, add up what was taken
void takeAll(SpmcQueue* queue, int total, atomic<int>* taken, atomic<int64_t>* sum) {
    while (taken->load() < total) {
        int64_t val = 0;
        bool popped = queue->pop(val);
        if (popped) {
            sum->fetch_add(val);
            taken->fetch_add(1);
        } else {
            yield();
        }
    }
}

// consumer thread of (2): follow the ring up to the last msg; every msg it reads
// must be whole (qty == 2 * price) and newer than the one before
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
            readPos = ring->numPublished() - 1; // lapped: jump to the newest msg
        }
    }
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // (1) test on one thread
    SpmcQueue queue(2);
    int64_t val = 0;
    check("queue-empty", !queue.pop(val));
    check("queue-full", queue.push(10) && queue.push(11) && !queue.push(12));
    check("queue-fifo", queue.pop(val) && val == 10 && queue.pop(val) && val == 11);

    // (1) test on threads: 2 consumers, each msg 1 .. total must be taken once
    int total = 100000;
    SpmcQueue jobs(4);
    atomic<int> taken(0);
    atomic<int64_t> sum(0);
    thread worker1(takeAll, &jobs, total, &taken, &sum);
    thread worker2(takeAll, &jobs, total, &taken, &sum);
    for (int i = 1; i <= total; i++) {
        bool pushed = jobs.push(i);
        while (!pushed) {
            yield();
            pushed = jobs.push(i);
        }
    }
    worker1.join();
    worker2.join();
    check("queue-each-once", taken.load() == total && sum.load() == 5000050000LL);

    // (2) test on one thread
    BroadcastRing ring(2);
    uint64_t readPos = 0;
    int64_t price = 0;
    int64_t qty = 0;
    check("ring-not-yet", ring.read(readPos, price, qty) == 0);
    ring.publish(100, 5);
    check("ring-read", ring.read(readPos, price, qty) == 1 && price == 100 && qty == 5);
    check("ring-moved-on", readPos == 1 && ring.read(readPos, price, qty) == 0);

    uint64_t slowPos = 0; // a second consumer that never read msg 0
    ring.publish(101, 6);
    ring.publish(102, 7); // msg 2 overwrites the slot of msg 0
    check("ring-lapped", ring.read(slowPos, price, qty) == -1);
    slowPos = ring.numPublished() - 1;
    check("ring-resync", ring.read(slowPos, price, qty) == 1 && price == 102);

    // (2) test on threads: 2 consumers follow msgs with price i, qty 2 * i
    BroadcastRing feed(8);
    bool ok1 = true;
    bool ok2 = true;
    thread strat1(follow, &feed, total - 1, &ok1);
    thread strat2(follow, &feed, total - 1, &ok2);
    for (int i = 0; i < total; i++) {
        feed.publish(i, 2 * i);
    }
    strat1.join();
    strat2.join();
    check("ring-no-torn-read", ok1 && ok2);
}
