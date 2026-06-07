// max-heap, T: O(nlogn), S: O(n)

#include <vector>
#include <queue> // std::priority_queue
#include <functional> // std::less

class Solution {
public:
    int lastStoneWeight(std::vector<int>& stones) {
        std::vector<int> buf;
        buf.reserve(stones.size());
        std::priority_queue<int, std::vector<int>> heap(std::less<int>{}, std::move(buf)); // max_heap

        for (int x : stones) { heap.push(x); }

        while (heap.size() > 1) {
            int y = heap.top(); heap.pop();
            int x = heap.top(); heap.pop();
            if (x != y) { heap.push(y - x); }
        }
        return heap.empty() ? 0 : heap.top();
    }
};

// y - x not x - y: y popped first, always the larger stone
