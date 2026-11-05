// Fixed-size thread pool (ByteDance prep doc: concurrency in Rounds 1-2): workers are
// started once; submit(job) queues an int job id and some worker runs handler(job).
// shutdown() (also run by the destructor) lets queued jobs finish, then joins workers.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;
using std::atomic;

// Workers sleep on a condition variable until a job arrives or the pool stops; a job
// runs outside the lock. T: O(1) per submit, S: O(workers + queued jobs).
class ThreadPool {
    // function every worker calls on a job id
    void (*handler)(int);
    // jobs not started yet, oldest first
    queue<int> jobs;
    // the worker threads
    vector<thread> workers;
    // guards jobs and stopping
    mutex m;
    // signaled when a job is queued or the pool stops
    condition_variable hasWork;
    // set by shutdown; workers exit once it is set and the queue is empty
    bool stopping = false;

    void workerLoop() {
        unique_lock<mutex> lock(m);
        // exit only when stopping and every queued job has been taken
        while (!stopping || !jobs.empty()) {
            // while, not if: wake-ups can be spurious, or another worker took the job
            while (!stopping && jobs.empty()) { hasWork.wait(lock); }
            if (jobs.empty()) { continue; }
            int job = jobs.front();
            jobs.pop();
            // run outside the lock so other workers can take jobs meanwhile
            lock.unlock();
            handler(job);
            lock.lock();
        }
    }

public:
    ThreadPool(int numWorkers, void (*handler)(int)) : handler(handler) {
        for (int i = 0; i < numWorkers; i++) {
            workers.push_back(thread(&ThreadPool::workerLoop, this));
        }
    }

    ~ThreadPool() { shutdown(); }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void submit(int job) {
        {
            lock_guard<mutex> lock(m);
            jobs.push(job);
        }
        hasWork.notify_one();
    }

    // safe to call twice: joined threads are no longer joinable
    void shutdown() {
        {
            lock_guard<mutex> lock(m);
            stopping = true;
        }
        hasWork.notify_all();
        for (thread& w : workers) {
            if (w.joinable()) { w.join(); }
        }
    }
};

atomic<long long> total{0};

void addToTotal(int job) { total += job; }

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ThreadPool pool(4, addToTotal);
    for (int job = 1; job <= 1000; job++) { pool.submit(job); }
    pool.shutdown();
    check(total == 500500, "all-jobs-run-once");

    total = 0;
    {
        ThreadPool scoped(2, addToTotal);
        for (int job = 1; job <= 10; job++) { scoped.submit(job); }
    }
    check(total == 55, "destructor-drains-queue");
}

// follow-up: return results -> submit(F) wraps shared_ptr<packaged_task<R()>> (move-only,
//   function<void()> needs copyable), returns its future
// follow-up: shutdownNow -> under lock clear jobs + set stopping, notify_all; running
//   jobs can't be preempted, they must poll a cancel flag
// follow-up: bounded queue / backpressure -> notFull cv, submit blocks or rejects when
//   full (as in LC 1188)
// follow-up: job throws -> catch in workerLoop; an exception escaping a thread calls
//   terminate() (packaged_task stores it in the future instead)
