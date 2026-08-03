// sort-by-start + one-pass

#include <vector>
#include <algorithm> // std::sort, std::max

class Solution {
public:
    std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
        std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
            return a[0] < b[0];
        });
        std::vector<std::vector<int>> out;
        out.reserve(intervals.size());

        for (const std::vector<int>& cur : intervals) {
            if (!out.empty() && cur[0] <= out.back()[1]) {
                out.back()[1] = std::max(out.back()[1], cur[1]);
            } else {
                out.push_back(cur);
            }
        }
        return out;
    }
};
