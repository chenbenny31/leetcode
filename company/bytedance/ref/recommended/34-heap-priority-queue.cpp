// ByteDance (FastPrep, phone): implement an int MIN priority queue without priority_queue
// or heap functions. Run operations PUSH values[i] / PEEK / POP left to right and return
// every PEEK / POP result. PEEK / POP never see an empty queue; duplicates allowed.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// Binary min-heap in an array: children of i are 2i+1 and 2i+2, every parent <= its
// children, so the minimum sits at index 0. T: O(log n) push / pop, O(1) top; S: O(n).
class MinHeap {
    // heap-ordered values
    vector<int> a;

    // move a[i] up while it is smaller than its parent
    void siftUp(int i) {
        while (i > 0 && a[(i - 1) / 2] > a[i]) {
            swap(a[(i - 1) / 2], a[i]);
            i = (i - 1) / 2;
        }
    }

    // move a[i] down to the smaller child until both children are >= it
    void siftDown(int i) {
        int n = a.size();
        // stop at a leaf: no left child
        while (2 * i + 1 < n) {
            int smallest = 2 * i + 1;
            int right = 2 * i + 2;
            if (right < n && a[right] < a[smallest]) {
                smallest = right;
            }
            if (a[i] <= a[smallest]) {
                break;
            }
            swap(a[i], a[smallest]);
            i = smallest;
        }
    }

public:
    void push(int x) {
        a.push_back(x);
        siftUp(a.size() - 1);
    }

    int top() const {
        return a[0];
    }

    // move the last element to the root, then sift it down
    int pop() {
        int result = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) {
            siftDown(0);
        }
        return result;
    }
};

// T: O(n log n) for n operations, S: O(n).
vector<int> runPriorityQueue(const vector<string>& operations,
                             const vector<int>& values) {
    MinHeap heap;
    vector<int> result;
    for (int i = 0; i < (int)operations.size(); i++) {
        if (operations[i] == "PUSH") {
            heap.push(values[i]);
        } else if (operations[i] == "PEEK") {
            result.push_back(heap.top());
        } else {
            result.push_back(heap.pop());
        }
    }
    return result;
}

void test(const string& label, const vector<string>& operations,
          const vector<int>& values, const vector<int>& want) {
    bool ok = runPriorityQueue(operations, values) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted-pops", {"PUSH", "PUSH", "PUSH", "PUSH", "POP", "POP", "POP", "POP"},
         {4, 1, 3, 2, 0, 0, 0, 0}, {1, 2, 3, 4});
    test("peek-dup", {"PUSH", "PUSH", "PEEK", "POP", "PEEK"}, {5, 5, 0, 0, 0}, {5, 5, 5});
    test("interleave", {"PUSH", "POP", "PUSH", "PUSH", "PEEK", "POP", "POP"},
         {2, 0, -1, 7, 0, 0, 0}, {2, -1, -1, 7});
}

// follow-up: build from n values -> heapify: siftDown(i) for i = n/2 - 1 down to 0, O(n)
// follow-up: delete / update by id -> pos[id] = heap index, updated on every swap; move
//   last into the slot, then siftUp + siftDown there, O(log n)
// follow-up: max-heap -> flip the two comparisons (or push -x and negate on pop)
// follow-up: median of a stream (LC 295) -> max-heap low + min-heap high, size diff <= 1
