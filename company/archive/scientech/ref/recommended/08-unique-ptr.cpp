// 08 · Hand-written unique_ptr — follow-up to 04-shared-ptr (smart pointer questions)
// UniquePtr<T> = sole owner of one heap object; it is deleted when the owner goes away
//
// clarify:
// copy -> not allowed (compile error); move -> ownership transfers
// custom deleter, arrays, make_unique -> not needed
// empty UniquePtr -> ptr = nullptr
// self-move-assignment -> must be safe

#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::move;
using std::exchange;

// core idea: one raw ptr; copy ops deleted, move takes the ptr and nulls the source,
// destructor deletes
// T: O(1) per op, S: one ptr
template <typename T>
class UniquePtr {
    T* ptr = nullptr; // the owned object, nullptr when empty

public:
    UniquePtr() = default;

    explicit UniquePtr(T* raw) : ptr(raw) {}

    // delete on nullptr is a no-op, so no check
    ~UniquePtr() { delete ptr; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.release()) {}

    // self-move is safe: release() empties *this, then reset() takes the same ptr back
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        reset(other.release());
        return *this;
    }

    // give up ownership without deleting; the caller owns the object now
    T* release() { return exchange(ptr, nullptr); }

    // own raw instead; swap in first, delete after (T's dtor may reach back into us)
    void reset(T* raw = nullptr) {
        T* old = exchange(ptr, raw);
        delete old;
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    explicit operator bool() const { return ptr != nullptr; }
};

// num of Probe objects destroyed so far
int destroyed = 0;

struct Probe {
    int value;
    explicit Probe(int v) : value(v) {}
    ~Probe() { destroyed++; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    {
        UniquePtr<Probe> first(new Probe(7));
        check(first && first->value == 7, "owns");

        UniquePtr<Probe> second = move(first);
        check(!first && (*second).value == 7 && destroyed == 0, "move-steals");

        UniquePtr<Probe> other(new Probe(9));
        other = move(second);
        check(!second && other->value == 7 && destroyed == 1, "move-assign-frees-old");

        Probe* raw = other.release();
        check(!other && destroyed == 1, "release-no-delete");

        other.reset(raw);
        other.reset(new Probe(3));
        check(other->value == 3 && destroyed == 2, "reset-frees-old");

        UniquePtr<Probe>& alias = other;
        other = move(alias);
        check(other->value == 3 && destroyed == 2, "self-move");
    }
    check(destroyed == 3, "dtor-frees");
}

// follow-up:
// 1. custom deleter -> template param D kept as a member; call d(ptr) instead of delete
// 2. arrays -> a UniquePtr<T[]> specialization that calls delete[] and has operator[]
// 3. make_unique -> variadic template: return UniquePtr<T>(new T(forward<Args>(args)...))
// 4. vs SharedPtr -> no count, no extra allocation, same size as a raw ptr
// 5. why noexcept on the moves -> vector moves on growth only if the move can't throw
// 6. why reset deletes last -> the old object's dtor must never see a half-updated owner
