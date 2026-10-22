// LC 703 Kth Largest Element in a Stream: KthLargest(k, nums) starts with nums;
// add(val) appends val and returns the kth largest value so far.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::greater;

// Min-heap capped at k holds the k largest values; its top is the kth largest.
// T: O(log k) per add, S: O(k).
class KthLargest {
    int k;
    priority_queue<int, vector<int>, greater<int>> heap;   // k largest values so far

public:
    KthLargest(int k, vector<int>& nums) : k(k) {
        for (int x : nums) { add(x); }
    }

    int add(int val) {
        heap.push(val);
        if ((int)heap.size() > k) { heap.pop(); }
        return heap.top();
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<int> start = {10, 20};
    KthLargest kth(2, start);
    check(kth.add(15) == 15, "grows-past-initial");
    check(kth.add(5) == 15, "smaller-ignored");
    check(kth.add(30) == 20, "larger-evicts");
    vector<int> none;
    KthLargest first(1, none);
    check(first.add(-3) == -3 && first.add(-7) == -3, "empty-start");
}
