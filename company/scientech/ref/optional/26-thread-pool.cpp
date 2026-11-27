// 26 · Thread pool — quant core (not reported)
// a fixed num of worker threads run tasks from one shared queue
// submit(task) queues a task; shutdown() runs what is queued, then stops the workers
//
// clarify:
// task -> no args, no return value: function<void()>; it must not throw
// queue -> unbounded; tasks start in the order they were queued, on any worker
// shutdown -> no new tasks, the queued ones still run, then the workers are joined
// compile -> add -pthread

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::function;
using std::bind;
using std::mutex;
using std::unique_lock;
using std::condition_variable;
using std::thread;
using std::atomic;

// core idea: the blocking queue pattern with workers as the consumers; a worker takes
// one task under the lock and runs it outside the lock
// T: submit O(1), S: O(queued tasks + workers)
class ThreadPool {
    vector<thread> workers;
    queue<function<void()>> tasks; // waiting tasks, oldest first
    bool stopping = false;
    mutex mtx;                     // guards tasks and stopping
    condition_variable hasWork;    // idle workers wait here

    void workLoop() {
        while (true) {
            function<void()> task;
            {
                unique_lock<mutex> lock(mtx);
                while (tasks.empty() && !stopping) { hasWork.wait(lock); }
                // stopping and nothing left
                if (tasks.empty()) { return; }
                task = tasks.front();
                tasks.pop();
            }
            // no lock held here, so the other workers keep going
            task();
        }
    }

public:
    ThreadPool(int numThreads) {
        for (int i = 0; i < numThreads; i++) {
            workers.push_back(thread(&ThreadPool::workLoop, this));
        }
    }

    ~ThreadPool() { shutdown(); }

    // false if the pool is shutting down
    bool submit(function<void()> task) {
        {
            unique_lock<mutex> lock(mtx);
            if (stopping) { return false; }
            tasks.push(task);
        }
        hasWork.notify_one();
        return true;
    }

    // safe to call twice: a joined thread is not joinable any more
    void shutdown() {
        {
            unique_lock<mutex> lock(mtx);
            stopping = true;
        }
        // notify_all: every idle worker must wake up and see stopping
        hasWork.notify_all();
        for (thread& worker : workers) {
            if (worker.joinable()) { worker.join(); }
        }
    }
};

// a task: add val to the shared sum
void addTo(atomic<long long>* sum, int val) {
    sum->fetch_add(val);
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    atomic<long long> sum(0);
    ThreadPool pool(4);

    bool accepted = true;
    for (int i = 1; i <= 1000; i++) {
        if (!pool.submit(bind(addTo, &sum, i))) { accepted = false; }
    }
    pool.shutdown();

    check(accepted && sum.load() == 500500, "all-ran-before-shutdown-returns");
    check(!pool.submit(bind(addTo, &sum, 1)), "submit-after-shutdown");
    pool.shutdown();
    check(sum.load() == 500500, "shutdown-twice");
}

// follow-up:
// 1. task returns a value -> packaged_task + future; submit returns the future
// 2. bounded queue -> a second condition variable notFull (18-blocking-queue)
// 3. stop now, drop the queued tasks -> on stopping, workers return without draining
// 4. a task throws -> try / catch around task() so the worker survives
// 5. less contention -> one queue per worker + work stealing
// 6. priorities -> priority_queue of (priority, task) instead of the FIFO queue
