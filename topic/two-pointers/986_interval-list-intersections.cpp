// two-pointers, T: O(m + n), S: O(1)

#include <vector>
#include <algorithm> // std::max, std::min

class Solution {
public:
    std::vector<std::vector<int>> intervalIntersection(
            std::vector<std::vector<int>>& firstList,
            std::vector<std::vector<int>>& secondList) {
        const int m = static_cast<int>(firstList.size());
        const int n = static_cast<int>(secondList.size());

        std::vector<std::vector<int>> out;
        out.reserve(m + n);
        int first = 0;
        int second = 0;

        while (first < m && second < n) {
            int lo = std::max(firstList[first][0], secondList[second][0]);
            int hi = std::min(firstList[first][1], secondList[second][1]);
            if (lo <= hi) { out.push_back({lo, hi}); }

            if (firstList[first][1] < secondList[second][1]) { first++; }
            else { second++; }
        }
        return out;
    }
};
