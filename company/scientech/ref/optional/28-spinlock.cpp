// spinlock: a lock that waits by looping instead of going to sleep
// lock(), tryLock(), unlock(); good only when the lock is held for a very short time

// usage:
// g++ -std=c++17 -Wall -Wextra -pthread 28-spinlock.cpp -o a && ./a
//
// SpinLock spin;  spin.lock();  ...a few instructions...  spin.unlock();

// clarify / assumptions
// not recursive: locking twice from the same thread hangs
// not fair: any waiting thread can win, no queue
// waiting burns a core; a mutex sleeps instead, but a sleep + wakeup costs far more
//   than a short spin -> spin when the critical section is a few instructions

// data:
// locked: true while some thread owns the lock
// exchange(true) sets it and returns the old value in one atomic step:
//   old value false = it was free and now it is mine

#include <atomic>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

using std::atomic;
using std::int64_t;
using std::cout;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;
using std::string;
using std::thread;
using std::vector;
using std::this_thread::yield;

class SpinLock {
private:
    atomic<bool> locked; // true while some thread owns the lock
public:
    SpinLock() : locked(false) {}

    void lock() {
        while (true) {
            bool wasLocked = locked.exchange(true, memory_order_acquire);
            if (!wasLocked) { return; } // it was free, now it is mine

            // wait with plain loads: an exchange is a write, and writes from every
            // waiter keep pulling the cache line away from the owner
            while (locked.load(memory_order_relaxed)) {
                yield(); // production: a cpu pause instruction instead
            }
        }
    }

    // one try, no waiting; return true if the lock is mine now
    bool tryLock() {
        bool wasLocked = locked.exchange(true, memory_order_acquire);
        return !wasLocked;
    }

    void unlock() {
        locked.store(false, memory_order_release);
    }
};

// each thread adds 1 to a plain (non-atomic) counter under the lock
void addMany(SpinLock* spin, int64_t* counter, int times) {
    for (int i = 0; i < times; i++) {
        spin->lock();
        (*counter)++;
        spin->unlock();
    }
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test on one thread
    SpinLock spin;
    check("try-free", spin.tryLock());
    check("try-held", !spin.tryLock());
    spin.unlock();
    check("try-after-unlock", spin.tryLock());
    spin.unlock();

    // test on 4 threads: without the lock some increments would be lost
    int64_t counter = 0;
    vector<thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.push_back(thread(addMany, &spin, &counter, 50000));
    }
    for (thread& t : threads) {
        t.join();
    }
    check("no-lost-update", counter == 200000);
}
