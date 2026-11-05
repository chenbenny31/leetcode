// Thread-safe LRU cache (ByteDance prep doc: concurrency + memory efficiency): get(key)
// returns the value or -1, put(key, value) inserts or updates and evicts the least
// recently used key when over capacity. Many threads call get / put at once.

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <utility>
#include <thread>
#include <mutex>
#include <functional>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::ref;
using std::unordered_map;

// LRU = list (recency order) + map (key -> list node), all behind one mutex. get also
// moves a node, so it is a write: a reader / writer lock would not help.
// T: O(1) average per call plus lock wait, S: O(capacity).
class ThreadSafeLRU {
    // max number of keys
    int capacity;
    // (key, value), most recently used at the front
    list<pair<int, int>> order;
    // pos[key] = node of key in order
    unordered_map<int, list<pair<int, int>>::iterator> pos;
    // guards order and pos
    mutex m;

public:
    ThreadSafeLRU(int capacity) : capacity(capacity) {}

    int get(int key) {
        lock_guard<mutex> lock(m);
        if (!pos.count(key)) { return -1; }
        // splice moves the node to the front without invalidating its iterator
        order.splice(order.begin(), order, pos[key]);
        return pos[key]->second;
    }

    void put(int key, int value) {
        lock_guard<mutex> lock(m);
        if (pos.count(key)) {
            pos[key]->second = value;
            order.splice(order.begin(), order, pos[key]);
            return;
        }
        order.push_front({key, value});
        pos[key] = order.begin();
        if ((int)order.size() > capacity) {
            pos.erase(order.back().first);
            order.pop_back();
        }
    }

    int size() {
        lock_guard<mutex> lock(m);
        return order.size();
    }
};

// each thread hammers its own key range, so final values are predictable
void worker(ThreadSafeLRU& cache, int base) {
    for (int round = 0; round < 200; round++) {
        for (int k = base; k < base + 5; k++) {
            cache.put(k, k * 10);
            cache.get(k);
        }
    }
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ThreadSafeLRU c(2);
    c.put(1, 10);
    c.put(2, 20);
    c.get(1);
    c.put(3, 30);
    check(c.get(2) == -1 && c.get(1) == 10 && c.get(3) == 30, "evict-least-recent");

    ThreadSafeLRU shared(8);
    vector<thread> threads;
    for (int t = 0; t < 4; t++) {
        threads.push_back(thread(worker, ref(shared), t * 100));
    }
    for (thread& t : threads) { t.join(); }
    bool valuesOk = true;
    for (int t = 0; t < 4; t++) {
        for (int k = t * 100; k < t * 100 + 5; k++) {
            int v = shared.get(k);
            if (v != -1 && v != k * 10) { valuesOk = false; }
        }
    }
    check(shared.size() == 8 && valuesOk, "four-threads-capacity-held");
}

// follow-up: less contention -> S shards {mutex, list, map}, shard = hash(key) % S,
//   capacity / S each; eviction becomes per-shard (approximate global LRU)
// follow-up: read-heavy -> CLOCK: get takes shared_lock and sets an atomic ref bit;
//   put / eviction take unique_lock, the hand clears bits until one is 0
// follow-up: slow load on miss -> load outside the lock; dedupe concurrent misses with
//   unordered_map<key, shared_future<V>> in flight (no thundering herd)
