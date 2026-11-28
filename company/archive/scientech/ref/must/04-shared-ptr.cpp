// 04 · Hand-written shared_ptr — smart pointer question (threads 988026, 938918)
// SharedPtr<T> = shared ownership: copies share one object and one count
// the object is deleted when its last owner goes away
//
// clarify:
// thread-safe count -> no (std uses an atomic count; follow-up 1)
// weak_ptr, custom deleter, make_shared, arrays -> not needed
// empty SharedPtr -> ptr = nullptr, use_count 0
// self-assignment -> must be safe
// typing order -> ctor, dtor, copy ctor first; then copy assign; then the two moves

#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;

// core idea: every owner holds the same two ptrs, the object and a heap int count;
// copy -> count++, destroy -> count--, delete both at 0; move steals, count unchanged
// T: O(1) per op, S: one int per managed object
template <typename T>
class SharedPtr {
    T* ptr = nullptr;
    int* cnt = nullptr; // num of owners, shared by all of them; nullptr when empty

    // give up my share; the last owner frees the object and the count
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
    SharedPtr() = default;

    explicit SharedPtr(T* raw) : ptr(raw) {
        if (raw != nullptr) { cnt = new int(1); }
    }

    ~SharedPtr() { release(); }

    SharedPtr(const SharedPtr& other) : ptr(other.ptr), cnt(other.cnt) {
        if (cnt != nullptr) { (*cnt)++; }
    }

    SharedPtr& operator=(const SharedPtr& other) {
        // self-assignment: release() first would free the object we are about to share
        if (this == &other) { return *this; }
        release();
        ptr = other.ptr;
        cnt = other.cnt;
        if (cnt != nullptr) { (*cnt)++; }
        return *this;
    }

    // noexcept: vector only moves its elements on growth when the move can't throw
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), cnt(other.cnt) {
        other.ptr = nullptr;
        other.cnt = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) { return *this; }
        release();
        ptr = other.ptr;
        cnt = other.cnt;
        other.ptr = nullptr;
        other.cnt = nullptr;
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    int use_count() const { return cnt == nullptr ? 0 : *cnt; }
    void reset() { release(); }
};

// num of Probe objects destroyed so far
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

        second = move(third);
        check(third.get() == nullptr && second.use_count() == 2, "move-assign");
    }
    check(destroyed == 2, "last-owner-frees");
}

// follow-up:
// 1. owners on several threads -> atomic<int> count; the object itself is still unguarded
// 2. weak_ptr -> control block {ptr, strong, weak}; the block lives until weak hits 0 too
// 3. ownership cycle (a <-> b never freed) -> make one direction a weak_ptr
// 4. make_shared -> one allocation holds the count and the object: one new, better cache
// 5. unique_ptr -> no count: delete the copy ops, a move steals the ptr (08)
// 6. copy-and-swap -> operator=(SharedPtr other) swaps ptr and cnt; one op for both
// 7. why explicit -> a raw ptr must not turn into an owner by an implicit conversion
// 8. new int(1) throws (bad_alloc) -> delete raw, then rethrow; the std one does that
