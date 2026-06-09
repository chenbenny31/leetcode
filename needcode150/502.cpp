// greedy + two-heaps, T: O(nlogn + klogn), S: O(n)

#include <vector>
#include <queue> // std::priority_queue
#include <algorithm> // std::sort
#include <utility> // std::pair

class Solution {
public:
    int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital) {
        int n = static_cast<int>(profits.size());

        std::vector<std::pair<int, int>> projects(n);
        for (int i = 0; i < n; i++) { projects[i] = {capital[i], profits[i]}; }
        std::sort(projects.begin(), projects.end());

        std::vector<int> buf;
        buf.reserve(n);
        std::priority_queue<int, std::vector<int>, std::less<int>> maxHeap(std::less<int>{}, std::move(buf));

        int idx = 0;
        for (int i = 0; i < k; i++) {
            while (idx < n && projects[idx].first <= w) {
                maxHeap.push(projects[idx].second);
                idx++;
            }
            if (maxHeap.empty()) { break; } // no affordable project
            w += maxHeap.top(); maxHeap.pop();
        }
        return w;
    }
};

// greedy logic: can always complete cheap but profitable projects and then try the greater capital ones
