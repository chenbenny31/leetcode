// SharedPtr<T> = shared ownership, copies share obj and +1 count
// obj is deleted when last owner goes

// thread-safe count not need -> use atomic if need
// no weak_ptr, custom deleter, make_shared, arrays
// empty SharedPtr -> ptr = nullptr with use_count=0
// self-assignment -> safe

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::move;

// every owner holds the same two ptr: the obj and heap int cnt
// copy -> cnt++, destory -> cnt--, delete both =0; move -> streal, count same
template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* cnt;

    void release() {
        if (cnt == nullptr) { return; }
        (*cnt)--;
        if (*cnt == 0) {
            delete ptr;
            delete cnt;
        }
        ptr = nullptr;
        cnt = nullptr;
    }

public:
    SharedPtr() : ptr(nullptr), cnt(nullptr) {}

    explicit SharedPtr(T* raw) : ptr(raw), cnt(nullptr) {
        if (raw != nullptr) { cnt = new int(1); }
    }

    ~SharedPtr() { release(); }

    // copy
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), cnt(other.cnt) {
        if (cnt != nullptr) { (*cnt)++; }
    }

    // move
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), cnt(other.cnt) {
        other.ptr = nullptr;
        other.cnt = nullptr;
    }

    // assign
    SharedPtr& operator=(const SharedPtr& other) {
        // self assign check, avoid 
        if (this == &other) { return *this; }
        release();
        ptr = other.ptr;
        cnt = other.cnt;
        if (cnt != nullptr) { (*cnt)++; }
        return *this;
    }

    T* get() const { return ptr; }

    T& operator*() const { return *ptr; }

    T* operator->() const { return ptr; }

    int use_count() const {
        if (cnt == nullptr) { return 0; }
        return *cnt;
    }

    void reset() { release(); }
};

// probe
int destroyed = 0;

struct Probe {
    int value;
    Probe(int v) : value(v) {}
    ~Probe() { destroyed++; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    {
        SharedPtr<Probe> first(new Probe(7));
        SharedPtr<Probe> second = first;
        check(first.use_count() == 2 && second->value == 7, "copy-shares");

        SharedPtr<Probe> third = move(first);
        check(first.get() == nullptr && third.use_count() == 2, "move-steals");

        SharedPtr<Probe> other(new Probe(9));
        other = third;
        check(destroyed == 1 && third.use_count() == 3, "assign-frees-old");

        SharedPtr<Probe>& alias = other;
        other = alias;
        check(other.use_count() == 3 && (*other).value == 7, "self-assign");

        second.reset();
        check(second.get() == nullptr && third.use_count() == 2, "reset");
    }
    check(destroyed == 2, "last-owner-frees");
}
