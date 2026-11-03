// ByteDance (FastPrep, phone): implement an integer MIN priority queue without the
// built-in priority_queue / heap functions. Process operations left to right:
//   PUSH -> insert values[i];  PEEK -> report the minimum;  POP -> remove and report it.
// Return every PEEK / POP result in order. PEEK / POP never see an empty queue;
// duplicates are allowed; values[i] is ignored for PEEK / POP.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// Binary min-heap stored in an array: children of i are 2i+1 and 2i+2, and every
// parent is <= its children, so the minimum sits at index 0.
class MinHeap {
    // heap-ordered values
    vector<int> a;

    // move a[i] up while it is smaller than its parent
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (a[parent] <= a[i]) {
                break;
            }
            swap(a[parent], a[i]);
            i = parent;
        }
    }

    // move a[i] down to the smaller child until both children are >= it
    void siftDown(int i) {
        int n = a.size();
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && a[left] < a[smallest]) {
                smallest = left;
            }
            if (right < n && a[right] < a[smallest]) {
                smallest = right;
            }
            if (smallest == i) {
                break;
            }
            swap(a[i], a[smallest]);
            i = smallest;
        }
    }

public:
    // T: O(log n)
    void push(int x) {
        a.push_back(x);
        siftUp(a.size() - 1);
    }

    // T: O(1)
    int top() const {
        return a[0];
    }

    // Move the last element to the root, then sift it down. T: O(log n)
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
