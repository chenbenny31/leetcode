// LC 1115 Print FooBar Alternately (ByteDance prep doc: concurrency in Rounds 1-2):
// one thread calls foo() and another calls bar(), each running n iterations; together
// they must produce "foobar" repeated n times.
// (LC passes printFoo / printBar callbacks; here both append to a shared string.)

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using std::cout;
using std::string;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::ref;

// A turn flag guarded by one mutex; each thread waits for its turn, prints, flips the
// flag and wakes the other. T: O(n) handoffs, S: O(1).
class FooBar {
    // iterations each thread runs
    int n;
    // true when it is foo's turn to print
    bool fooTurn = true;
    // guards fooTurn and the shared output
    mutex m;
    // signaled whenever the turn flips
    condition_variable turnChanged;

public:
    FooBar(int n) : n(n) {}

    void foo(string& out) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(m);
            while (!fooTurn) { turnChanged.wait(lock); }
            out += "foo";
            fooTurn = false;
            turnChanged.notify_one();
        }
    }

    void bar(string& out) {
        for (int i = 0; i < n; i++) {
            unique_lock<mutex> lock(m);
            while (fooTurn) { turnChanged.wait(lock); }
            out += "bar";
            fooTurn = true;
            turnChanged.notify_one();
        }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FooBar three(3);
    string out;
    // start bar first: it must still wait for foo
    thread b(&FooBar::bar, &three, ref(out));
    thread f(&FooBar::foo, &three, ref(out));
    f.join();
    b.join();
    check(out == "foobarfoobarfoobar", "bar-starts-first");

    FooBar many(1000);
    string longOut;
    thread f2(&FooBar::foo, &many, ref(longOut));
    thread b2(&FooBar::bar, &many, ref(longOut));
    f2.join();
    b2.join();
    string want;
    for (int i = 0; i < 1000; i++) { want += "foobar"; }
    check(longOut == want, "thousand-rounds");
}

// follow-up: busy-wait instead -> atomic<bool> fooTurn + this_thread::yield() spin; no
//   lock, but burns CPU while waiting; cv sleeps
// follow-up: semaphores (C++20) -> binary_semaphore fooSem(1), barSem(0); foo: acquire
//   fooSem, print, release barSem; bar mirrored
// follow-up: k threads in turn (e.g. LC 1116) -> int turn; thread i waits turn == i, then
//   turn = (i + 1) % k; notify_all (notify_one may wake the wrong thread)
