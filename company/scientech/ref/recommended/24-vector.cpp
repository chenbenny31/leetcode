// hand-written vector: a growing array of T
// push_back (amortized O(1)), pop_back, operator[], size, capacity, copy and move

// usage:
// g++ -std=c++17 -Wall -Wextra 24-vector.cpp -o a && ./a
//
// Vector<string> names;  names.push_back("AAPL");  names[0];  names.size();

// clarify / assumptions
// T can be any copyable type, it need not have a default constructor
// capacity doubles when full (1, 2, 4, ...); it never shrinks
// operator[] and pop_back do not check the index, same as std::vector
// moving a T does not throw (so a grow can't fail half way)

// data:
// data: raw memory for cap objects; only the first len slots hold a live object
// raw memory + placement new, not new T[cap]: new T[cap] would construct cap objects
// right away and needs a default constructor

#include <cstddef>
#include <new>
#include <string>
#include <utility>
#include <iostream>

using std::move;
using std::size_t;
using std::cout;
using std::string;

template <typename T>
class Vector {
private:
    T* data; // raw memory for cap objects, slots [0, len) are live
    size_t len;
    size_t cap;

    // move every object into a bigger block, then free the old one
    void grow(size_t newCap) {
        T* bigger = (T*)operator new(newCap * sizeof(T));
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
        for (size_t i = 0; i < len; i++) {
            data[i].~T();
        }
        operator delete(data);
    }
public:
    Vector() : data(nullptr), len(0), cap(0) {}

    ~Vector() {
        destroyAll();
    }

    // copy: own block, own copy of every object
    Vector(const Vector& other) : data(nullptr), len(0), cap(0) {
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
        Vector copy(other); // if this throws, *this is untouched
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

    // amortized O(1): a grow costs O(len), but doubling makes it rare
    void push_back(const T& val) {
        T copy(val); // val may be one of my own elements, and grow would free it
        if (len == cap) {
            size_t newCap = (cap == 0) ? 1 : 2 * cap;
            grow(newCap);
        }
        new (data + len) T(move(copy));
        len++;
    }

    void pop_back() {
        len--;
        data[len].~T();
    }

    T& operator[](size_t i) { return data[i]; }
    size_t size() { return len; }
    size_t capacity() { return cap; }
};

// num of Probe objects alive right now: must be back to 0 at the end
int alive = 0;

struct Probe {
    int value;
    Probe(int v) : value(v) { alive++; }
    Probe(const Probe& other) : value(other.value) { alive++; }
    ~Probe() { alive--; }
};

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Vector<string> names;
    check("empty", names.size() == 0 && names.capacity() == 0);

    names.push_back("AAPL");
    names.push_back("MSFT");
    names.push_back("NVDA");
    check("push", names.size() == 3 && names[0] == "AAPL" && names[2] == "NVDA");
    check("doubles", names.capacity() == 4);

    names.push_back(names[0]);
    names.push_back(names[0]); // len == cap here: grows while pushing its own element
    check("push-own-element", names.size() == 5 && names[4] == "AAPL");

    Vector<string> copy(names);
    copy[0] = "TSLA";
    check("copy-is-separate", names[0] == "AAPL" && copy[0] == "TSLA");

    Vector<string> moved(move(copy));
    check("move-takes-block", moved.size() == 5 && copy.size() == 0);

    names = moved;
    check("copy-assign", names[0] == "TSLA" && names.size() == 5);
    names.pop_back();
    check("pop", names.size() == 4);

    {
        Vector<Probe> probes;
        probes.push_back(Probe(1));
        probes.push_back(Probe(2));
        probes.push_back(Probe(3));
        Vector<Probe> other(probes);
        other.pop_back();
        check("alive-while-in-use", alive == 5 && probes[2].value == 3);
    }
    check("all-destroyed", alive == 0);
}
