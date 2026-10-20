// Apple (actual interview, 2026-10): fixed-capacity ring buffer on a vector with
// push (at the back), pop (from the front), front() and back(). push on a full buffer
// is rejected; front/back/pop need a non-empty buffer.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class RingBuffer {
    vector<int> data;   // circular storage; capacity = data.size()
    int head = 0;       // index of the front element
    int count = 0;      // elements stored; avoids the head == tail full/empty ambiguity

public:
    RingBuffer(int capacity) : data(capacity) {}

    // write one past the back, wrapping around. T: O(1).
    bool push(int x) {
        int cap = data.size();
        if (count == cap) { return false; }
        data[(head + count) % cap] = x;
        count++;
        return true;
    }

    // drop the front by advancing head. T: O(1).
    bool pop() {
        if (count == 0) { return false; }
        head = (head + 1) % (int)data.size();
        count--;
        return true;
    }

    int front() { return data[head]; }

    int back() { return data[(head + count - 1) % (int)data.size()]; }

    bool empty() { return count == 0; }

    bool full() { return count == (int)data.size(); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RingBuffer rb(2);
    check(rb.empty() && rb.push(1) && rb.push(2) && rb.full(), "fill");
    check(!rb.push(3), "reject-full");
    check(rb.pop() && rb.push(3) && rb.front() == 2 && rb.back() == 3, "wrap");
    check(rb.pop() && rb.pop() && !rb.pop() && rb.empty(), "drain");
}
