// 34 · Spinlock — quant core (not reported)
// a lock that waits by looping instead of going to sleep: lock(), tryLock(), unlock()
// good only when the lock is held for a very short time
//
// clarify:
// recursive -> no: locking twice from the same thread hangs
// fair -> no: any waiting thread can win, there is no queue
// why spin -> a sleep + wakeup costs far more than a few instructions of spinning
// compile -> add -pthread

#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <thread>

using std::cout;
using std::string;
using std::vector;
using std::atomic;
using std::memory_order_acquire;
using std::memory_order_relaxed;
using std::memory_order_release;
using std::thread;
using std::this_thread::yield;

// core idea: exchange(true) sets the flag and returns the old value in one atomic step;
// old value false = it was free and now it is mine
// T: O(1) when free, else it spins; S: O(1)
class SpinLock {
    atomic<bool> locked{false}; // true while some thread owns the lock

public:
    void lock() {
        while (true) {
            if (!locked.exchange(true, memory_order_acquire)) { return; }
            // wait with plain loads: an exchange is a write, and writes from every
            // waiter keep pulling the cache line away from the owner
            while (locked.load(memory_order_relaxed)) { yield(); }
        }
    }

    // one try, no waiting; true if the lock is mine now
    bool tryLock() { return !locked.exchange(true, memory_order_acquire); }

    void unlock() { locked.store(false, memory_order_release); }
};

// each thread adds 1 to a plain (non-atomic) counter under the lock
void addMany(SpinLock* spin, long long* counter, int times) {
    for (int i = 0; i < times; i++) {
        spin->lock();
        (*counter)++;
        spin->unlock();
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    SpinLock spin;
    check(spin.tryLock() && !spin.tryLock(), "try-free-then-held");
    spin.unlock();
    check(spin.tryLock(), "try-after-unlock");
    spin.unlock();

    long long counter = 0;
    vector<thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.push_back(thread(addMany, &spin, &counter, 50000));
    }
    for (thread& t : threads) { t.join(); }
    check(counter == 200000, "no-lost-update");
}

// follow-up:
// 1. production wait -> a cpu pause instruction (_mm_pause) instead of yield
// 2. spin a while, then sleep -> hybrid lock: after k spins fall back to a mutex / futex
// 3. fairness -> ticket lock: take a number, wait until now-serving reaches it
// 4. RAII -> lock_guard<SpinLock> works once the methods are named lock / unlock
// 5. atomic_flag -> test_and_set / clear, the one type guaranteed lock-free
// 6. spinlock vs mutex -> spin when the critical section is shorter than a context switch
