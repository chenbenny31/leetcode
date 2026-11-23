// thread pool: a fixed num of worker threads run tasks from one shared queue
// submit(task) queues a task; shutdown() runs what is queued, then stops the workers

// usage:
// g++ -std=c++17 -Wall -Wextra -pthread 18-thread-pool.cpp -o a && ./a
//
// ThreadPool pool(4);  pool.submit(task);  pool.shutdown();

// clarify / assumptions
// a task takes no args and returns nothing: function<void()>
// the queue is unbounded; tasks run in the order they were queued, on any worker
// shutdown: no new tasks, tasks already queued still run, then the workers are joined
// a task must not throw (an exception would end its worker thread)

// data:
// tasks: waiting tasks, oldest first
// mtx guards tasks and stopping; hasWork is where idle workers sleep

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

using std::atomic;
using std::bind;
using std::condition_variable;
using std::function;
using std::int64_t;
using std::cout;
using std::lock_guard;
using std::mutex;
using std::queue;
using std::string;
using std::thread;
using std::unique_lock;
using std::vector;

class ThreadPool {
private:
    vector<thread> workers;
    queue<function<void()>> tasks; // waiting tasks, oldest first
    bool stopping;
    mutex mtx; // guards tasks and stopping
    condition_variable hasWork; // idle workers wait here

    // each worker: take one task under the lock, run it outside the lock
    void workLoop() {
        while (true) {
            function<void()> task;
            {
                unique_lock<mutex> lock(mtx);
                while (tasks.empty() && !stopping) { // loop: a wakeup can be spurious
                    hasWork.wait(lock);
                }
                if (tasks.empty()) { return; } // stopping and nothing left
                task = tasks.front();
                tasks.pop();
            }
            task(); // no lock held: other workers keep going
        }
    }
public:
    ThreadPool(int numThreads) : stopping(false) {
        for (int i = 0; i < numThreads; i++) {
            workers.push_back(thread(&ThreadPool::workLoop, this));
        }
    }

    ~ThreadPool() {
        shutdown();
    }

    // return false if the pool is shutting down, T: O(1)
    bool submit(function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);
            if (stopping) { return false; }
            tasks.push(task);
        }
        hasWork.notify_one();
        return true;
    }

    // safe to call twice: a joined thread is not joinable any more
    void shutdown() {
        {
            lock_guard<mutex> lock(mtx);
            stopping = true;
        }
        hasWork.notify_all(); // every idle worker must wake up and see stopping
        for (thread& worker : workers) {
            if (worker.joinable()) { worker.join(); }
        }
    }
};

// a task: add val to the shared sum
void addTo(atomic<int64_t>* sum, int64_t val) {
    sum->fetch_add(val);
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    atomic<int64_t> sum(0);
    ThreadPool pool(4);

    bool accepted = true;
    for (int i = 1; i <= 1000; i++) {
        bool queued = pool.submit(bind(addTo, &sum, i));
        if (!queued) { accepted = false; }
    }
    pool.shutdown();

    check("all-accepted", accepted);
    check("all-ran-before-shutdown-returns", sum.load() == 500500);
    check("submit-after-shutdown", !pool.submit(bind(addTo, &sum, 1)));
    pool.shutdown();
    check("shutdown-twice", sum.load() == 500500);
}
