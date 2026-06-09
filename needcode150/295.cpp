// two heaps, T: O(logn) add, O(1) findMedian, S: O(n)

#include <vector>
#include <queue> // std::priority_queue
#include <functional> // std::greater, std::less

class MedianFinder {
private:
    std::priority_queue<int> maxHeap; // lower half, top is max of lower
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // upper half, top is min of upper

public:
    MedianFinder() {
        std::vector<int> lo_buf, hi_buf;
        lo_buf.reserve(50001);
        hi_buf.reserve(50001);
        std::priority_queue<int> lo(std::less<int>{}, std::move(lo_buf));
        std::priority_queue<int, std::vector<int>, std::greater<int>> hi(std::greater<int>{}, std::move(hi_buf));
        maxHeap = std::move(lo);
        minHeap = std::move(hi);
    }

    void addNum(int num) {
        maxHeap.push(num);
        minHeap.push(maxHeap.top());
        maxHeap.pop();

        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        if (maxHeap.size() > minHeap.size()) { return maxHeap.top(); }
        return (maxHeap.top() + static_cast<double>(minHeap.top())) / 2.0;
    }
};

// inavriant: only need mid-smaller and mid-greater or single median element
// addNum logic: push to maxHeap first, ensure new element routed correctly via minHeap.push(maxHeap.top())
