// 03 · Hand-written shared_ptr — C++ round (smart pointers, ctors, writing a class)
// SharedPtr<T> = shared ownership: copies share one object + one count
// the object is deleted when its last owner goes away
// write: ctor, dtor, copy ctor / assign, move ctor / assign, get, *, ->, use_count, reset
//
// clarify:
// thread-safe count -> no (std uses an atomic count; follow-up)
// weak_ptr, custom deleter, make_shared, arrays -> not needed
// empty SharedPtr -> ptr = nullptr, use_count 0
// self-assignment -> must be safe

#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;

// core idea: every owner holds the same two pointers: the object and a heap int count
// copy -> count + 1; destroy -> count - 1, delete both at 0; move -> steal, count same
// T: O(1) per op, S: one int per managed object
template <typename T>
class SharedPtr {
    // managed object; nullptr = empty
    T* ptr;
    // num of owners of ptr, shared by all of them; nullptr when empty
    int* count;

    // give up this owner's share; the last owner frees the object and the count
    void release() {
        if (count == nullptr) { return; }
        (*count)--;
        if (*count == 0) {
            delete ptr;
            delete count;
        }
        ptr = nullptr;
        count = nullptr;
    }

public:
    SharedPtr() : ptr(nullptr), count(nullptr) {}

    // explicit: no silent T* -> SharedPtr conversion (two owners of one raw ptr)
    explicit SharedPtr(T* raw) : ptr(raw), count(nullptr) {
        if (raw != nullptr) { count = new int(1); }
    }

    ~SharedPtr() { release(); }

    // copy: share the object, one more owner
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
        if (count != nullptr) { (*count)++; }
    }

    // move: take both pointers, count unchanged; other becomes empty
    // noexcept -> vector moves (not copies) elements when it grows
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), count(other.count) {
        other.ptr = nullptr;
        other.count = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        // without this check release() could free the object we are about to share
        if (this == &other) { return *this; }
        release();
        ptr = other.ptr;
        count = other.count;
        if (count != nullptr) { (*count)++; }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) { return *this; }
        release();
        ptr = other.ptr;
        count = other.count;
        other.ptr = nullptr;
        other.count = nullptr;
        return *this;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    int use_count() const {
        if (count == nullptr) { return 0; }
        return *count;
    }

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
    }
    check(destroyed == 2, "last-owner-frees");
}

// follow-up:
// thread safety -> count = atomic<int>; the object itself is still not guarded
// control block -> std keeps {strong count, weak count, deleter} in one heap block
// make_shared -> one allocation for block + object; no raw new at the call site
// weak_ptr -> object dies at strong == 0, block at weak == 0; lock() -> SharedPtr
// cycle a -> b -> a -> counts never hit 0 (leak); make one direction a weak_ptr
// two SharedPtr from one raw ptr -> two counts -> double delete; copy the SharedPtr
// copy-and-swap -> one operator=(SharedPtr other) { swap ptr, count } covers copy + move
// rule of 5 -> owning a raw resource: dtor, copy ctor / assign, move ctor / assign
// unique_ptr -> no count; copy is deleted, move steals:
/*
UniquePtr(const UniquePtr&) = delete;
UniquePtr& operator=(const UniquePtr&) = delete;
UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) { other.ptr = nullptr; }
~UniquePtr() { delete ptr; }
*/
