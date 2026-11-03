// ByteDance (1p3a 1189695, intern infra round): implement a simplified shared_ptr,
// SharedPtr<T>. Several SharedPtr objects can own one heap object; it is deleted when
// the last owner goes away. Support: construction from a raw pointer, copy / move
// construction, copy / move assignment (self-assignment safe), destructor, reset(),
// get(), operator*, operator->, use_count().
// Follow-ups (discussion):
// - Thread safety: make the count an atomic<int>; the owner whose fetch_sub returned 1
//   deletes. The pointed-to object itself is not made thread safe.
// - weak_ptr: the control block also needs a weak count; the object dies when the
//   strong count hits 0, the control block when strong + weak both hit 0.
// - make_shared: allocate the object and the control block in one allocation.
#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;

// ptr and count travel together: every owner of an object shares one heap-allocated
// counter (the control block). Copying adds an owner, moving transfers ownership, and
// release() drops this owner - the last one frees both the object and the counter.
template <typename T>
class SharedPtr {
    // the managed object; nullptr when this SharedPtr owns nothing
    T* ptr;
    // shared control block = number of owners of ptr; nullptr when empty
    int* count;

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

    // stealing the pointers leaves the count unchanged
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

    // drop the current object (if any) and own p instead
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
    static int alive;
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
        check(a.use_count() == 1 && a->value == 5 && (*a).value == 5, "own-one");
        {
            SharedPtr<Tracked> b = a;
            check(a.use_count() == 2 && b.get() == a.get(), "copy-shares");
        }
        check(a.use_count() == 1 && Tracked::alive == 1, "copy-scope-ends");

        SharedPtr<Tracked> c = move(a);
        bool moved = a.get() == nullptr && a.use_count() == 0 && c.use_count() == 1;
        check(moved, "move-ctor");

        SharedPtr<Tracked>& alias = c;
        c = alias;
        check(c.use_count() == 1 && Tracked::alive == 1, "self-assign");

        SharedPtr<Tracked> d(new Tracked(8));
        d = c;
        bool freed = Tracked::alive == 1 && d->value == 5 && c.use_count() == 2;
        check(freed, "assign-frees-old");

        SharedPtr<Tracked> e;
        e = move(d);
        check(d.use_count() == 0 && e.use_count() == 2, "move-assign");

        c.reset(new Tracked(9));
        bool replaced = Tracked::alive == 2 && c.use_count() == 1 && e.use_count() == 1;
        check(replaced, "reset-new");
        e.reset();
        check(Tracked::alive == 1 && e.get() == nullptr, "reset-empty");
    }
    check(Tracked::alive == 0, "last-owner-frees");
}
