// min-heap with size k, T: O(logk) per add, S: O(k)

#include <vector>
#include <queue> // std::priority_queue
#include <functional> // std::greater

class KthLargest {
private:
    int k;
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap; // min heap

public:
    KthLargest(int k, std::vector<int>& nums) : k(k) {
        std::vector<int> buf;
        buf.reserve(k + 1);
        std::priority_queue<int, std::vector<int>, std::greater<int>> tmp(std::greater<int>(), std::move(buf));
        heap = std::move(tmp);

        for (int x : nums) {
            heap.push(x);
            if (static_cast<int>(heap.size()) > k) { heap.pop(); }
        }
    }

    int add(int val) {
        heap.push(val);
        if (static_cast<int>(heap.size()) > k) { heap.pop(); }
        return heap.top();
    }
};

// invariant: heap always hold k largest elements seen so far
// min-heap not max-heap: max_heap top is largest, need pop k - 1 times for k-th, O(klogn) per query
