// LC 1188 Design Bounded Blocking Queue (ByteDance prep doc: concurrency in Rounds 1-2):
// enqueue(x) blocks while the queue is full, dequeue() blocks while it is empty, size()
// returns the current count. Safe to call from many producer / consumer threads.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;
using std::ref;
using std::sort;

// One mutex guards the queue; two condition variables let producers wait for space and
// consumers wait for an item. T: O(1) per call plus waiting, S: O(capacity).
class BoundedBlockingQueue {
    // max number of items
    int capacity;
    // items in FIFO order
    queue<int> items;
    // guards items
    mutex m;
    // signaled after a dequeue: there is space now
    condition_variable notFull;
    // signaled after an enqueue: there is an item now
    condition_variable notEmpty;

public:
    BoundedBlockingQueue(int capacity) : capacity(capacity) {}

    void enqueue(int x) {
        unique_lock<mutex> lock(m);
        // while, not if: wake-ups can be spurious, or another producer took the slot
        while ((int)items.size() == capacity) { notFull.wait(lock); }
        items.push(x);
        notEmpty.notify_one();
    }

    int dequeue() {
        unique_lock<mutex> lock(m);
        while (items.empty()) { notEmpty.wait(lock); }
        int x = items.front();
        items.pop();
        notFull.notify_one();
        return x;
    }

    int size() {
        lock_guard<mutex> lock(m);
        return items.size();
    }
};

void produce(BoundedBlockingQueue& q, int first, int count) {
    for (int i = 0; i < count; i++) { q.enqueue(first + i); }
}

void consume(BoundedBlockingQueue& q, int count, vector<int>& got) {
    for (int i = 0; i < count; i++) { got.push_back(q.dequeue()); }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    BoundedBlockingQueue q(2);
    vector<int> got;
    thread producer(produce, ref(q), 0, 100);
    thread consumer(consume, ref(q), 100, ref(got));
    producer.join();
    consumer.join();
    bool inOrder = (int)got.size() == 100;
    for (int i = 0; i < (int)got.size(); i++) {
        if (got[i] != i) { inOrder = false; }
    }
    check(inOrder && q.size() == 0, "fifo-while-blocking");

    BoundedBlockingQueue shared(3);
    vector<vector<int>> parts(3);
    vector<thread> threads;
    for (int t = 0; t < 3; t++) {
        threads.push_back(thread(produce, ref(shared), t * 100, 100));
        threads.push_back(thread(consume, ref(shared), 100, ref(parts[t])));
    }
    for (thread& t : threads) { t.join(); }
    vector<int> all;
    for (const vector<int>& p : parts) { all.insert(all.end(), p.begin(), p.end()); }
    sort(all.begin(), all.end());
    bool exactlyOnce = (int)all.size() == 300;
    for (int i = 0; i < (int)all.size(); i++) {
        if (all[i] != i) { exactlyOnce = false; }
    }
    check(exactlyOnce, "three-by-three-exactly-once");
}

// follow-up: one cv instead of two -> must notify_all: notify_one may wake a waiter of
//   the wrong kind and the wakeup is lost; two cvs make notify_one safe
// follow-up: shutdown -> bool closed under m; close() sets it + notify_all on both cvs;
//   enqueue fails when closed, dequeue returns optional<int>{} once closed && empty
// follow-up: timeout -> notFull.wait_for(lock, d, pred) returns false on timeout, so
//   offer() returns false; same for poll()
// follow-up: single producer / consumer, lock-free -> ring buffer with atomic head/tail,
//   writer stores its index with release, reader loads it with acquire
