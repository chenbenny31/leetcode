// ByteDance (1p3a, intern infra round): implement SharedPtr<T>, a simplified shared_ptr:
// raw-pointer ctor, copy / move ctor and assignment (self-assignment safe), dtor,
// reset(), get(), operator*, operator->, use_count(). Last owner deletes the object.

#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;

// Owners share one heap counter; copy adds an owner, move transfers, last release frees.
// T: O(1) per op, S: O(1) per object.
template <typename T>
class SharedPtr {
    // the managed object; nullptr when this SharedPtr owns nothing
    T* ptr;
    // shared control block = number of owners of ptr; nullptr when empty
    int* count;

    // drop this owner; the last one frees the object and the counter
    void release() {
        if (count == nullptr) {
            return;
        }
        *count -= 1;
        if (*count == 0) {
            delete ptr;
            delete count;
        }
        ptr = nullptr;
        count = nullptr;
    }

public:
    SharedPtr() : ptr(nullptr), count(nullptr) {}

    explicit SharedPtr(T* p) : ptr(p), count(nullptr) {
        if (p != nullptr) {
            count = new int(1);
        }
    }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
        if (count != nullptr) {
            *count += 1;
        }
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        // releasing first would free the object when this is its only owner
        if (this == &other) {
            return *this;
        }
        release();
        ptr = other.ptr;
        count = other.count;
        if (count != nullptr) {
            *count += 1;
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        release();
        ptr = other.ptr;
        count = other.count;
        other.ptr = nullptr;
        other.count = nullptr;
        return *this;
    }

    ~SharedPtr() {
        release();
    }

    void reset(T* p = nullptr) {
        release();
        if (p != nullptr) {
            ptr = p;
            count = new int(1);
        }
    }

    T* get() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    int use_count() const {
        if (count == nullptr) {
            return 0;
        }
        return *count;
    }
};

// counts live instances so the tests can see exactly when objects are destroyed
struct Tracked {
    // number of Tracked objects alive right now
    static int alive;
    // payload to check which object a pointer sees
    int value;

    explicit Tracked(int v) : value(v) {
        alive++;
    }

    ~Tracked() {
        alive--;
    }
};

int Tracked::alive = 0;

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    {
        SharedPtr<Tracked> a(new Tracked(5));
        bool shared = false;
        {
            SharedPtr<Tracked> b = a;
            shared = a.use_count() == 2 && b.get() == a.get() && (*b).value == 5;
        }
        check(shared && a.use_count() == 1 && Tracked::alive == 1, "copy-shares");

        SharedPtr<Tracked> c = move(a);
        SharedPtr<Tracked> e;
        e = move(c);
        bool moved = a.get() == nullptr && c.use_count() == 0 && e.use_count() == 1;
        check(moved && e->value == 5, "move-ctor-assign");

        SharedPtr<Tracked>& alias = e;
        e = alias;
        SharedPtr<Tracked> d(new Tracked(8));
        d = e;
        bool freed = Tracked::alive == 1 && d->value == 5 && e.use_count() == 2;
        check(freed, "self-and-copy-assign");

        e.reset(new Tracked(9));
        d.reset();
        bool reset = Tracked::alive == 1 && e->value == 9 && d.get() == nullptr;
        check(reset, "reset");
    }
    check(Tracked::alive == 0, "last-owner-frees");
}

// follow-up: thread safety -> count becomes atomic<int>; the owner whose fetch_sub
//   returned 1 deletes; the pointee itself is not made thread safe
// follow-up: reference cycle (a->b, b->a) leaks -> make one side weak_ptr; control block
//   gets a weak count: object dies at strong == 0, block at strong + weak == 0
// follow-up: SharedPtr(p) twice on one raw p -> two control blocks, double delete;
//   copy the first SharedPtr, or enable_shared_from_this
// follow-up: make_shared -> object and control block in one allocation
