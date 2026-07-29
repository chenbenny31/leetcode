// binary-search on answer

#include <vector>
#include <algorithm> // std::max_element

class Solution {
public:
    int minEatingSpeed(std::vector<int>& piles, int h) {
        int lo = 1;
        int hi = *std::max_element(piles.begin(), piles.end());

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (canFinish(piles, h, mid)) { hi = mid; }
            else { lo = mid + 1; }
        }
        return lo;
    }

private:
    bool canFinish(std::vector<int>& piles, int h, int mid) {
        int need = 0;
        for (int p : piles) {
            need += (p + mid - 1) / mid; // ceil-div
        }
        return need <= h;
    }
};
