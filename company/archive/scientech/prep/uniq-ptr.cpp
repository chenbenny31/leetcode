#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;
using std::forward;

template <typename T>
class UniquePtr {
private:
    T* ptr = nullptr;

public:
    UniquePtr() = default;
    explicit UniquePtr(T* p) : ptr(p) {}
    ~UniquePtr() { delete ptr; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // move
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.release()) {}

    // assign
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        reset(other.release());
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    explicit operator bool() const { return ptr != nullptr; }

    // give up ownership and return old
    T* release() {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    // own p stead, delete the old
    void reset(T* p = nullptr) {
        T* old = ptr;
        ptr = p;
        delete old;
    }
};

template <typename T, typename... Args>
UniquePtr<T> makeUnique(Args&&... args) {
    return UniquePtr<T>(new T(forward<Args>(args)...));
}

struct Tracker {
    static inline int alive = 0;
    int val;
    Tracker(int v) : val(v) { alive++; }
    ~Tracker() { alive--; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    {
        UniquePtr<Tracker> a = makeUnique<Tracker>(7);
        UniquePtr<Tracker> b = move(a);
        check(!a && b->val == 7 && Tracker::alive == 1, "move-ctor");
        b = makeUnique<Tracker>(8);
        check((*b).val == 8 && Tracker::alive == 1, "move-assign-frees-old");
        Tracker* raw = b.release();
        check(!b && Tracker::alive == 1, "release-keeps-object");
        b.reset(raw);
    }
    check(Tracker::alive == 0, "dtor-frees");
}
