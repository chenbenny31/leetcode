// 31 · Hand-written vector — quant core (reported at Jump, not here)
// a growing array of T: push_back (amortized O(1)), pop_back, operator[], size,
// capacity, copy and move
//
// clarify:
// T -> any copyable type; it need not have a default ctor
// growth -> capacity doubles when full (1, 2, 4, ...), never shrinks
// operator[] and pop_back -> no idx check, same as the std one
// moving a T -> does not throw, so a grow can't fail half way

#include <iostream>
#include <string>
#include <cstddef>
#include <new>
#include <utility>

using std::cout;
using std::string;
using std::size_t;
using std::move;

// core idea: raw memory for cap objects, only slots [0, len) hold a live object;
// placement new builds one in place, an explicit dtor call ends it
// raw memory, not new T[cap]: that would build cap objects now and need a default ctor
// T: push_back O(1) amortized (a grow costs O(len), doubling makes it rare), S: O(cap)
template <typename T>
class Vector {
    T* data = nullptr; // raw memory for cap objects, slots [0, len) are live
    size_t len = 0;
    size_t cap = 0;

    // move every object into a bigger block, then free the old one
    void grow(size_t newCap) {
        T* bigger = static_cast<T*>(operator new(newCap * sizeof(T)));
        for (size_t i = 0; i < len; i++) {
            new (bigger + i) T(move(data[i]));
            data[i].~T();
        }
        operator delete(data);
        data = bigger;
        cap = newCap;
    }

    // destroy the live objects, then free the memory
    void destroyAll() {
        for (size_t i = 0; i < len; i++) { data[i].~T(); }
        operator delete(data);
    }

public:
    Vector() = default;

    ~Vector() { destroyAll(); }

    // copy: own block, own copy of every object
    Vector(const Vector& other) {
        if (other.len == 0) { return; }
        grow(other.len);
        for (size_t i = 0; i < other.len; i++) {
            new (data + i) T(other.data[i]);
            len++;
        }
    }

    // move: take the block, leave other empty
    Vector(Vector&& other) noexcept : data(other.data), len(other.len), cap(other.cap) {
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) { return *this; }
        // copy first: if it throws, *this is untouched
        Vector copy(other);
        *this = move(copy);
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) { return *this; }
        destroyAll();
        data = other.data;
        len = other.len;
        cap = other.cap;
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
        return *this;
    }

    void push_back(const T& val) {
        // val may be one of my own elements, and grow would free it
        T copy(val);
        if (len == cap) { grow(cap == 0 ? 1 : 2 * cap); }
        new (data + len) T(move(copy));
        len++;
    }

    void pop_back() {
        len--;
        data[len].~T();
    }

    T& operator[](size_t i) { return data[i]; }
    size_t size() const { return len; }
    size_t capacity() const { return cap; }
};

// num of Probe objects alive right now: must be back to 0 at the end
int alive = 0;

struct Probe {
    int value;
    Probe(int v) : value(v) { alive++; }
    Probe(const Probe& other) : value(other.value) { alive++; }
    ~Probe() { alive--; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Vector<string> names;
    names.push_back("AAPL");
    names.push_back("MSFT");
    names.push_back("NVDA");
    check(names.size() == 3 && names[0] == "AAPL" && names[2] == "NVDA", "push");
    check(names.capacity() == 4, "doubles");

    names.push_back(names[0]);
    names.push_back(names[0]);
    check(names.size() == 5 && names[4] == "AAPL", "push-own-element-while-growing");

    Vector<string> copy(names);
    copy[0] = "TSLA";
    check(names[0] == "AAPL" && copy[0] == "TSLA", "copy-is-separate");

    Vector<string> moved(move(copy));
    check(moved.size() == 5 && copy.size() == 0, "move-takes-block");

    names = moved;
    names.pop_back();
    check(names[0] == "TSLA" && names.size() == 4, "copy-assign-then-pop");

    {
        Vector<Probe> probes;
        probes.push_back(Probe(1));
        probes.push_back(Probe(2));
        Vector<Probe> other(probes);
        other.pop_back();
        check(alive == 3 && probes[1].value == 2, "alive-while-in-use");
    }
    check(alive == 0, "all-destroyed");
}

// follow-up:
// 1. emplace_back -> variadic template, forward the args into placement new
// 2. push_back(T&&) -> a second overload that moves val in, no copy
// 3. a T whose move can throw -> copy instead (move_if_noexcept) to keep the old block
// 4. reserve(n) -> grow(n) when n > cap; shrink_to_fit -> grow(len)
// 5. why growth factor 2 -> amortized O(1): total copies <= 2n; 1.5 reuses freed blocks
// 6. iterators -> begin() = data, end() = data + len; a grow invalidates them
