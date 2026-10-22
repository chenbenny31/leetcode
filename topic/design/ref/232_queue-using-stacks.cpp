// LC 232 Implement Queue using Stacks: FIFO queue with push, pop, peek, empty,
// built from two stacks only.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

class MyQueue {
    vector<int> in;    // stack of new pushes, newest on top
    vector<int> out;   // stack in pop order, oldest on top; refilled only when empty

    // moving only when out is empty keeps the order and pays O(1) per element
    void shift() {
        if (!out.empty()) { return; }
        while (!in.empty()) {
            out.push_back(in.back());
            in.pop_back();
        }
    }

public:
    // T: O(1)
    void push(int x) {
        in.push_back(x);
    }

    // two stacks, lazy transfer. T: O(1) amortized, S: O(n).
    int pop() {
        shift();
        int x = out.back();
        out.pop_back();
        return x;
    }

    // T: O(1) amortized
    int peek() {
        shift();
        return out.back();
    }

    // T: O(1)
    bool empty() {
        return in.empty() && out.empty();
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    MyQueue q;
    check(q.empty(), "empty");
    q.push(1);
    q.push(2);
    check(q.peek() == 1 && q.pop() == 1, "fifo");
    q.push(3);
    check(q.pop() == 2 && q.pop() == 3, "interleave");
    check(q.empty(), "drained");
}
