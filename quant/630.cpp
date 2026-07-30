// greedy + heap

#include <vector>
#include <algorithm> // std::sort
#include <queue> // std::priority_queue

class Solution {
public:
    int scheduleCourse(std::vector<std::vector<int>>& courses) {
        std::sort(courses.begin(), courses.end(), [](const std::vector<int>& a, std::vector<int>& b){ return a[1] < b[1]; }); // sort by incre ddl

        std::vector<int> buf;
        buf.reserve(courses.size());
        std::priority_queue<int, std::vector<int>, std::less<int>> maxHeap(std::less<int>{}, std::move(buf));

        long long time = 0;
        for (auto& c : courses) {
            int dur = c[0];
            int ddl = c[1];

            time += dur;
            maxHeap.push(dur);

            if (time > ddl) {
                time -= maxHeap.top(); maxHeap.pop();
            }
        }
        return static_cast<int>(maxHeap.size());
    }
};
