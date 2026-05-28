// binary search on answer, T: O(n * log(m)) - m: max(piles), S: O(1)

#include <vector>
#include <algorithm>

class Solution {
public:
    int minEatingSpeed(std::vector<int>& piles, int h) {
        int lo = 1;
        int hi = *std::max_element(piles.begin(), piles.end());

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (can_finish(piles, h, mid)) { hi = mid; }
            else { lo = mid + 1; }
        }
        return lo;
    }

private:
    bool can_finish(std::vector<int>& piles, int h, int speed) {
        long long hours = 0;
        for (int p : piles) { hours += (p + speed - 1) / speed; }
        return hours <= h;
    }
};

// ceil div without std::ceil: (p + speed - 1) / speed
// long long for hours: max hours = 10^9 / 1 * 10^4 = 10^13
